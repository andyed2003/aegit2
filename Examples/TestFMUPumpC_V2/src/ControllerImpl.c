#include <string.h>
#include "common.h"

fmi_Component modelInstances[MaxFMUInstances]; // start of with an empty array of components
int conInstanceCount = 0;

/* ---------------------------------------------------------------------------*
 * Implementation of the FMI interface based on functions and macros to
 * be defined by the includer of this file. 
 * If FMI_COSIMULATION is defined, this implements "FMI for Co-Simulation 1.0",
 * otherwise "FMI for Model Exchange 1.0".
 * The "FMI for Co-Simulation 1.0", implementation assumes that exactly the 
 * following capability flags are set to fmiTrue:
 *    canHandleVariableCommunicationStepSize, i.e. fmiDoStep step size can vary
 *    canHandleEvents, i.e. fmiDoStep step size can be zero
 * and all other capability flags are set to default, i.e. to fmiFalse or 0.
 *
 * Revision history
 *  07.02.2010 initial version for "Model Exchange 1.0" released in FMU SDK 1.0
 *  05.03.2010 bug fix: fmiSetString now copies the passed string argument
 *     and fmiFreeModelInstance frees all string copies
 *  11.12.2010 replaced calloc by functions.allocateMemory in fmiInstantiateModel
 *  04.08.2011 added support for "FMI for Co-Simulation 1.0"
 *  
 * (c) 2011 QTronic GmbH 
 * ---------------------------------------------------------------------------*/

#ifndef max
#define max(a,b) ((a)>(b) ? (a) : (b))
#endif

static fmiBoolean nullPointer(fmi_Component* comp, const char* f,
		const char* arg, const void* p) {
	if (!p) {
		comp->state = modelError;
		comp->functions.logger(comp, comp->instanceName, fmiError, "error",
				"%s: Invalid argument %s = NULL.", f, arg);
		return fmiTrue;
	}
	return fmiFalse;
}

static fmiBoolean vrOutOfRange(fmi_Component* comp, const char* f,
		fmiValueReference vr, int end) {
	if (vr >= end) {
		comp->functions.logger(comp, comp->instanceName, fmiError, "error",
				"%s: Illegal value reference %u.", f, vr);
		comp->state = modelError;
		return fmiTrue;
	}
	return fmiFalse;
}

static fmiStatus ControllerImpl_setString(fmiComponent comp,
		fmiValueReference vr, fmiString value) {
	return fmiSetString(comp, &vr, 1, &value);
}

static fmiBoolean invalidState(fmi_Component* comp, const char* f,
		int statesExpected) {
	if (!comp)
		return fmiTrue;
	if (!(comp->state & statesExpected)) {
		comp->state = modelError;
		comp->functions.logger(comp, comp->instanceName, fmiError, "error",
				"%s: Illegal call sequence.", f);
		return fmiTrue;
	}
	return fmiFalse;
}

// fname is fmiTerminate or fmiTerminateSlave
static fmiStatus terminate(char* fname, fmiComponent c) {
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, fname, modelInitialized))
		return fmiError;
	if (comp->loggingOn)
		comp->functions.logger(c, comp->instanceName, fmiOK, "log", fname);
	comp->state = modelTerminated;
	return fmiOK;
}

// fname is freeModelInstance of freeSlaveInstance
void freeInstance(char* fname, fmiComponent c) {
	fmi_Component* comp = (fmi_Component *) c;
	if (!comp)
		return;
	if (comp->loggingOn)
		comp->functions.logger(c, comp->instanceName, fmiOK, "log", fname);
	if (comp->r)
		comp->functions.freeMemory(comp->r);
	if (comp->i)
		comp->functions.freeMemory(comp->i);
	if (comp->b)
		comp->functions.freeMemory(comp->b);
	if (comp->s) {
		int i;
		for (i = 0; i < NUMBER_OF_STRINGS; i++) {
			if (comp->s[i])
				comp->functions.freeMemory((void*) comp->s[i]);
		}
		comp->functions.freeMemory(comp->s);
	}
	comp->functions.freeMemory(comp);
}

const char* ControllerImpl_fmiGetVersion() {
	return fmiVersion;
}

fmiStatus ControllerImpl_fmiSetDebugLogging(fmiComponent c,
		fmiBoolean loggingOn, size_t size, const fmiString str[]) {
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiSetDebugLogging", not_modelError))
		return fmiError;
	if (comp->loggingOn)
		comp->functions.logger(c, comp->instanceName, fmiOK, "log",
				"fmiSetDebugLogging: loggingOn=%d", loggingOn);
	comp->loggingOn = loggingOn;
	return fmiOK;
}

fmiStatus ControllerImpl_fmiSetReal(fmiComponent c,
		const fmiValueReference vr[], size_t nvr, const fmiReal value[]) {
	int i;
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiSetReal", modelInstantiated | modelInitialized))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiSetReal", "vr[]", vr))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiSetReal", "value[]", value))
		return fmiError;
	if (comp->loggingOn)
		comp->functions.logger(c, comp->instanceName, fmiOK, "log",
				"fmiSetReal: nvr = %d", nvr);
	// no check wether setting the value is allowed in the current state
	for (i = 0; i < nvr; i++) {
		if (vrOutOfRange(comp, "fmiSetReal", vr[i], NUMBER_OF_REALS))
			return fmiError;
		if (comp->loggingOn)
			comp->functions.logger(c, comp->instanceName, fmiOK, "log",
					"fmiSetReal: #r%d# = %.16g", vr[i], value[i]);
		comp->r[vr[i]] = value[i];
	}
	return fmiOK;
}

fmiStatus ControllerImpl_fmiSetBoolean(fmiComponent c,
		const fmiValueReference vr[], size_t nvr, const fmiBoolean value[]) {
	int i;
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiSetBoolean",
			modelInstantiated | modelInitialized))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiSetBoolean", "vr[]", vr))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiSetBoolean", "value[]", value))
		return fmiError;
	if (comp->loggingOn)
		comp->functions.logger(c, comp->instanceName, fmiOK, "log",
				"fmiSetBoolean: nvr = %d", nvr);
	for (i = 0; i < nvr; i++) {
		if (vrOutOfRange(comp, "fmiSetBoolean", vr[i], NUMBER_OF_BOOLEANS))
			return fmiError;
		if (comp->loggingOn)
			comp->functions.logger(c, comp->instanceName, fmiOK, "log",
					"fmiSetBoolean: #b%d# = %s", vr[i],
					value[i] ? "true" : "false");
		comp->b[vr[i]] = value[i];
	}
	return fmiOK;
}

fmiStatus ControllerImpl_fmiSetString(fmiComponent c,
		const fmiValueReference vr[], size_t nvr, const fmiString value[]) {
	int i;
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiSetString",
			modelInstantiated | modelInitialized))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiSetString", "vr[]", vr))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiSetString", "value[]", value))
		return fmiError;
	if (comp->loggingOn)
		comp->functions.logger(c, comp->instanceName, fmiOK, "log",
				"fmiSetString: nvr = %d", nvr);
	for (i = 0; i < nvr; i++) {
		const char* string = comp->s[vr[i]];
		if (vrOutOfRange(comp, "fmiSetString", vr[i], NUMBER_OF_STRINGS))
			return fmiError;
		if (comp->loggingOn)
			comp->functions.logger(c, comp->instanceName, fmiOK, "log",
					"fmiSetString: #s%d# = '%s'", vr[i], value[i]);
		if (nullPointer(comp, "fmiSetString", "value[i]", value[i]))
			return fmiError;
		if (string == NULL || strlen(string) < strlen(value[i])) {
			if (string)
				comp->functions.freeMemory((void*) string);
			comp->s[vr[i]] = comp->functions.allocateMemory(
					1 + strlen(value[i]), sizeof(char));
			if (!comp->s[vr[i]]) {
				comp->state = modelError;
				comp->functions.logger(NULL, comp->instanceName, fmiError,
						"error", "fmiSetString: Out of memory.");
				return fmiError;
			}
		}
		strcpy((char*) comp->s[vr[i]], value[i]);
	}
	return fmiOK;
}

// called by fmiGetReal, fmiGetContinuousStates and fmiGetDerivatives
static fmiReal getReal(fmi_Component* comp, fmiValueReference vr, size_t size,
		fmiReal value[]) {
	return -1;
}

fmiStatus ControllerImpl_fmiGetReal(fmiComponent c,
		const fmiValueReference vr[], size_t nvr, fmiReal value[]) {
#if NUMBER_OF_REALS>0
	int i;
#endif
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiGetReal", not_modelError))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiGetReal", "vr[]", vr))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiGetReal", "value[]", value))
		return fmiError;
#if NUMBER_OF_REALS>0
	for (i=0; i<nvr; i++) {
		if (vrOutOfRange(comp, "fmiGetReal", vr[i], NUMBER_OF_REALS))
		return fmiError;
		value[i] = getReal(comp, vr[i], nvr, value); // to be implemented by the includer of this file
		if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
				"fmiGetReal: #r%u# = %.16g", vr[i], value[i]);
	}
#endif
	return fmiOK;
}

fmiStatus ControllerImpl_fmiGetInteger(fmiComponent c,
		const fmiValueReference vr[], size_t nvr, fmiInteger value[]) {
	int i;
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiGetInteger", not_modelError))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiGetInteger", "vr[]", vr))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiGetInteger", "value[]", value))
		return fmiError;
	for (i = 0; i < nvr; i++) {
		if (vrOutOfRange(comp, "fmiGetInteger", vr[i], NUMBER_OF_INTEGERS))
			return fmiError;
		value[i] = comp->i[vr[i]];
		if (comp->loggingOn)
			comp->functions.logger(c, comp->instanceName, fmiOK, "log",
					"fmiGetInteger: #i%u# = %d", vr[i], value[i]);
	}
	return fmiOK;
}

fmiStatus ControllerImpl_fmiGetBoolean(fmiComponent c,
		const fmiValueReference vr[], size_t nvr, fmiBoolean value[]) {
	int i;
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiGetBoolean", not_modelError))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiGetBoolean", "vr[]", vr))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiGetBoolean", "value[]", value))
		return fmiError;
	for (i = 0; i < nvr; i++) {
		if (vrOutOfRange(comp, "fmiGetBoolean", vr[i], NUMBER_OF_BOOLEANS))
			return fmiError;
		value[i] = comp->b[vr[i]];
		if (comp->loggingOn)
			comp->functions.logger(c, comp->instanceName, fmiOK, "log",
					"fmiGetBoolean: #b%u# = %s", vr[i],
					value[i] ? "true" : "false");
	}
	return fmiOK;
}

fmiStatus ControllerImpl_fmiGetString(fmiComponent c,
		const fmiValueReference vr[], size_t nvr, fmiString value[]) {
	int i;
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiGetString", not_modelError))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiGetString", "vr[]", vr))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiGetString", "value[]", value))
		return fmiError;
	for (i = 0; i < nvr; i++) {
		if (vrOutOfRange(comp, "fmiGetString", vr[i], NUMBER_OF_STRINGS))
			return fmiError;
		value[i] = comp->s[vr[i]];
		if (comp->loggingOn)
			comp->functions.logger(c, comp->instanceName, fmiOK, "log",
					"fmiGetString: #s%u# = '%s'", vr[i], value[i]);
	}
	return fmiOK;
}

// WE always do co-simulation
//#ifdef FMI_COSIMULATION
// ---------------------------------------------------------------------------
// FMI functions: only for FMI Co-Simulation 1.0
// ---------------------------------------------------------------------------

const char* ControllerImpl_fmiGetTypesPlatform() {
	return fmiTypesPlatform;
}
fmiStatus ControllerImpl_fmiTerminateSlave(fmiComponent c) {
	return terminate("fmiTerminateSlave", c);
}

// does nothing here - we use the initialisationsListGenerator
static void setStartValues(fmi_Component *comp) {

}

fmiStatus ControllerImpl_fmiResetSlave(fmiComponent c) {
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiResetSlave", modelInitialized))
		return fmiError;
	if (comp->loggingOn)
		comp->functions.logger(c, comp->instanceName, fmiOK, "log",
				"fmiResetSlave");
	comp->state = modelInstantiated;
	setStartValues(comp); // to be implemented by the includer of this file
	return fmiOK;
}

void ControllerImpl_fmiFreeSlaveInstance(fmiComponent c) {
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiFreeSlaveInstance", modelTerminated))
		return;
	freeInstance("fmiFreeSlaveInstance", c);
}

fmiStatus ControllerImpl_fmiSetRealInputDerivatives(fmiComponent c,
		const fmiValueReference vr[], size_t nvr, const fmiInteger order[],
		const fmiReal value[]) {
	fmi_Component* comp = (fmi_Component *) c;
	fmiCallbackLogger log = comp->functions.logger;
	if (invalidState(comp, "fmiSetRealInputDerivatives", modelInitialized))
		return fmiError;
	if (comp->loggingOn)
		log(c, comp->instanceName, fmiOK, "log",
				"fmiSetRealInputDerivatives: nvr= %d", nvr);
	log(NULL, comp->instanceName, fmiError, "warning",
			"fmiSetRealInputDerivatives: ignoring function call."
					" This model cannot interpolate inputs: canInterpolateInputs=\"fmiFalse\"");
	return fmiWarning;
}

fmiStatus ControllerImpl_fmiGetRealOutputDerivatives(fmiComponent c,
		const fmiValueReference vr[], size_t nvr, const fmiInteger order[],
		fmiReal value[]) {
	int i;
	fmi_Component* comp = (fmi_Component *) c;
	fmiCallbackLogger log = comp->functions.logger;
	if (invalidState(comp, "fmiGetRealOutputDerivatives", modelInitialized))
		return fmiError;
	if (comp->loggingOn)
		log(c, comp->instanceName, fmiOK, "log",
				"fmiGetRealOutputDerivatives: nvr= %d", nvr);
	log(NULL, comp->instanceName, fmiError, "warning",
			"fmiGetRealOutputDerivatives: ignoring function call."
					" This model cannot compute derivatives of outputs: MaxOutputDerivativeOrder=\"0\"");
	for (i = 0; i < nvr; i++)
		value[i] = 0;
	return fmiWarning;
}

fmiStatus ControllerImpl_fmiCancelStep(fmiComponent c) {
	fmi_Component* comp = (fmi_Component *) c;
	fmiCallbackLogger log = comp->functions.logger;
	if (invalidState(comp, "fmiCancelStep", modelInitialized))
		return fmiError;
	if (comp->loggingOn)
		log(c, comp->instanceName, fmiOK, "log", "fmiCancelStep");
	log(NULL, comp->instanceName, fmiError, "error",
			"fmiCancelStep: Can be called when fmiDoStep returned fmiPending."
					" This is not the case.");
	return fmiError;
}

static fmiStatus getStatus(char* fname, fmiComponent c, const fmiStatusKind s) {
	const char* statusKind[3] = { "fmiDoStepStatus", "fmiPendingStatus",
			"fmiLastSuccessfulTime" };
	fmi_Component* comp = (fmi_Component *) c;
	fmiCallbackLogger log = comp->functions.logger;
	if (invalidState(comp, fname, modelInstantiated | modelInitialized))
		return fmiError;
	if (comp->loggingOn)
		log(c, comp->instanceName, fmiOK, "log", "$s: fmiStatusKind = %s",
				fname, statusKind[s]);
	switch (s) {
	case fmiDoStepStatus:
		log(NULL, comp->instanceName, fmiError, "error",
				"%s: Can be called with fmiDoStepStatus when fmiDoStep returned fmiPending."
						" This is not the case.", fname);
		break;
	case fmiPendingStatus:
		log(NULL, comp->instanceName, fmiError, "error",
				"%s: Can be called with fmiPendingStatus when fmiDoStep returned fmiPending."
						" This is not the case.", fname);
		break;
	case fmiLastSuccessfulTime:
		log(NULL, comp->instanceName, fmiError, "error",
				"%s: Can be called with fmiLastSuccessfulTime when fmiDoStep returned fmiDiscard."
						" This is not the case.", fname);
		break;
	}
	return fmiError;
}

fmiStatus ControllerImpl_fmiGetStatus(fmiComponent c, const fmiStatusKind s,
		fmiStatus* value) {
	return getStatus("fmiGetStatus", c, s);
}

fmiStatus ControllerImpl_fmiGetRealStatus(fmiComponent c, const fmiStatusKind s,
		fmiReal* value) {
	return getStatus("fmiGetRealStatus", c, s);
}

fmiStatus ControllerImpl_fmiGetIntegerStatus(fmiComponent c,
		const fmiStatusKind s, fmiInteger* value) {
	return getStatus("fmiGetIntegerStatus", c, s);
}

fmiStatus ControllerImpl_fmiGetBooleanStatus(fmiComponent c,
		const fmiStatusKind s, fmiBoolean* value) {
	return getStatus("fmiGetBooleanStatus", c, s);
}

fmiStatus ControllerImpl_fmiGetStringStatus(fmiComponent c,
		const fmiStatusKind s, fmiString* value) {
	return getStatus("fmiGetStringStatus", c, s);
}
//#endif // all

//// array of value references of states
//#if NUMBER_OF_REALS>0
//// Linux: declare vrStates to be static so that we get the local definition.
//static fmiValueReference vrStates[NUMBER_OF_STATES] = STATES;
//#endif

//static fmiBoolean invalidState(fmi_Component* comp, const char* f, int statesExpected){
//    if (!comp)
//        return fmiTrue;
//    if (!(comp->state & statesExpected)) {
//        comp->state = modelError;
//        comp->functions.logger(comp, comp->instanceName, fmiError, "error",
//                "%s: Illegal call sequence.", f);
//        return fmiTrue;
//    }
//    return fmiFalse;
//}

//// fname is fmiInstantiateModel or fmiInstantiateSlave
//static fmiComponent instantiateModel(char* fname, fmiString instanceName, fmiString GUID,
//        fmiCallbackFunctions functions, fmiBoolean loggingOn) {
//    fmi_Component* comp;
//    if (!functions.logger)
//        return NULL;
//    if (!functions.allocateMemory || !functions.freeMemory){
//        functions.logger(NULL, instanceName, fmiError, "error",
//                "%s: Missing callback function.", fname);
//        return NULL;
//    }
//    if (!instanceName || strlen(instanceName)==0) {
//        functions.logger(NULL, instanceName, fmiError, "error",
//                "%s: Missing instance name.", fname);
//        return NULL;
//    }
//    if (strcmp(GUID, MODEL_GUID)) {
//        functions.logger(NULL, instanceName, fmiError, "error",
//                "%s: Wrong GUID %s. Expected %s.", fname, GUID, MODEL_GUID);
//        return NULL;
//    }
//    comp = (fmi_Component *)functions.allocateMemory(1, sizeof(fmi_Component));
//    if (comp) {
//        comp->r = functions.allocateMemory(NUMBER_OF_REALS,    sizeof(fmiReal));
//        comp->i = functions.allocateMemory(NUMBER_OF_INTEGERS, sizeof(fmiInteger));
//        comp->b = functions.allocateMemory(NUMBER_OF_BOOLEANS, sizeof(fmiBoolean));
//        comp->s = functions.allocateMemory(NUMBER_OF_STRINGS,  sizeof(fmiString));
//        comp->isPositive = functions.allocateMemory(NUMBER_OF_EVENT_INDICATORS, sizeof(fmiBoolean));
//    }
//    if (!comp || !comp->r || !comp->i || !comp->b || !comp->s || !comp->isPositive) {
//        functions.logger(NULL, instanceName, fmiError, "error",
//                "%s: Out of memory.", fname);
//        return NULL;
//    }
//    if (comp->loggingOn) comp->functions.logger(NULL, instanceName, fmiOK, "log",
//            "%s: GUID=%s", fname, GUID);
//    comp->instanceName = instanceName;
//    comp->GUID = GUID;
//    comp->functions = functions;
//    comp->loggingOn = loggingOn;
//    comp->state = modelInstantiated;
//    setStartValues(comp); // to be implemented by the includer of this file
//    return comp;
//}

// fname is fmiInitialize or fmiInitializeSlave
//static fmiStatus ControllerImpl_init(char* fname, fmiComponent c, fmiBoolean toleranceControlled, fmiReal relativeTolerance,
//    fmiEventInfo* eventInfo) {
//    fmi_Component* comp = (fmi_Component *)c;
//    if (invalidState(comp, fname, modelInstantiated))
//         return fmiError;
//    if (nullPointer(comp, fname, "eventInfo", eventInfo))
//         return fmiError;
//    if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//        "%s: toleranceControlled=%d relativeTolerance=%g",
//        fname, toleranceControlled, relativeTolerance);
//    eventInfo->iterationConverged  = fmiTrue;
//    eventInfo->stateValueReferencesChanged = fmiFalse;
//    eventInfo->stateValuesChanged  = fmiFalse;
//    eventInfo->terminateSimulation = fmiFalse;
//    eventInfo->upcomingTimeEvent   = fmiFalse;
//    initialize(comp, eventInfo); // to be implemented by the includer of this file
//    comp->state = modelInitialized;
//    return fmiOK;
//}

fmiStatus ControllerImpl_fmiSetInteger(fmiComponent c,
		const fmiValueReference vr[], size_t nvr, const fmiInteger value[]) {
	int i;
	fmi_Component* comp = (fmi_Component *) c;
	if (invalidState(comp, "fmiSetInteger",
			modelInstantiated | modelInitialized))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiSetInteger", "vr[]", vr))
		return fmiError;
	if (nvr > 0 && nullPointer(comp, "fmiSetInteger", "value[]", value))
		return fmiError;
	if (comp->loggingOn)
		comp->functions.logger(c, comp->instanceName, fmiOK, "log",
				"fmiSetInteger: nvr = %d", nvr);
	for (i = 0; i < nvr; i++) {
		if (vrOutOfRange(comp, "fmiSetInteger", vr[i], NUMBER_OF_INTEGERS))
			return fmiError;
		if (comp->loggingOn)
			comp->functions.logger(c, comp->instanceName, fmiOK, "log",
					"fmiSetInteger: #i%d# = %d", vr[i], value[i]);
		comp->i[vr[i]] = value[i];
	}
	return fmiOK;
}

//fmiComponent fmiInstantiateSlave(fmiString  instanceName, fmiString  GUID,
//    fmiString  fmuLocation, fmiString  mimeType, fmiReal timeout, fmiBoolean visible,
//    fmiBoolean interactive, fmiCallbackFunctions functions, fmiBoolean loggingOn) {
//    // ignoring arguments: fmuLocation, mimeType, timeout, visible, interactive
//    return instantiateModel("fmiInstantiateSlave", instanceName, GUID, functions, loggingOn);
//}

//fmiStatus ControllerImpl_fmiInitializeSlave(fmiComponent c, fmiReal tStart, fmiBoolean StopTimeDefined, fmiReal tStop) {
//    fmi_Component* comp = (fmi_Component *)c;
//    fmiBoolean toleranceControlled = fmiFalse;
//    fmiReal relativeTolerance = 0;
//    fmiStatus flag = fmiOK;
//    comp->eventInfo.iterationConverged = 0;
//    while (flag==fmiOK && !comp->eventInfo.iterationConverged) {
//        // ignoring arguments: tStart, StopTimeDefined, tStop
//        flag = init("fmiInitializeSlave", c, toleranceControlled, relativeTolerance, &comp->eventInfo);
//    }
//    return flag;
//}

//fmiStatus ControllerImpl_fmiDoStep(fmiComponent c, fmiReal currentCommunicationPoint,
//    fmiReal communicationStepSize, fmiBoolean newStep) {
//    fmi_Component* comp = (fmi_Component *)c;
//    fmiCallbackLogger log = comp->functions.logger;
//    double h = communicationStepSize / 10;
//#if NUMBER_OF_EVENT_INDICATORS>0  || NUMBER_OF_REALS>0
//    int i;
//#endif
//    int k;
//    const int n = 10; // how many Euler steps to perform for one do step
//#if NUMBER_OF_REALS>0
//    double prevState[max(NUMBER_OF_STATES, 1)];
//#endif
//#if NUMBER_OF_EVENT_INDICATORS>0
//    double prevEventIndicators[max(NUMBER_OF_EVENT_INDICATORS, 1)];
//    int stateEvent = 0;
//#endif
//
//    if (invalidState(comp, "fmiDoStep", modelInitialized))
//         return fmiError;
//
//    if (comp->loggingOn) log(c, comp->instanceName, fmiOK, "log", "fmiDoStep: "
//       "currentCommunicationPoint = %g, ",
//       "communicationStepSize = %g, ",
//       "newStep = fmi%s",
//       currentCommunicationPoint, communicationStepSize, newStep ? "True" : "False");
//
//    // Treat also case of zero step, i.e. during an event iteration
//    if (communicationStepSize == 0) {
//        return fmiOK;
//    }
//
//#if NUMBER_OF_EVENT_INDICATORS>0
//    // initialize previous event indcators with current values
//    for (i=0; i<NUMBER_OF_EVENT_INDICATORS; i++) {
//        prevEventIndicators[i] = getEventIndicator(comp, i);
//    }
//#endif
//
//    // break the step into n steps and do forward Euler.
//    comp->time = currentCommunicationPoint;
//    for (k=0; k<n; k++) {
//        comp->time += h;
//
//#if NUMBER_OF_REALS>0
//        for (i=0; i<NUMBER_OF_STATES; i++) {
//            prevState[i] = comp->r[vrStates[i]];
//        }
//        for (i=0; i<NUMBER_OF_STATES; i++) {
//            fmiValueReference vr = vrStates[i];
//            comp->r[vr] += h * getReal(comp, vr+1, nvr, value); // forward Euler step
//        }
//#endif
//
//#if NUMBER_OF_EVENT_INDICATORS>0
//        // check for state event
//        for (i=0; i<NUMBER_OF_EVENT_INDICATORS; i++) {
//            double ei = getEventIndicator(comp, i);
//            if (ei * prevEventIndicators[i] < 0) {
//                if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//                    "fmiDoStep: state event at %g, z%d crosses zero -%c-", comp->time, i, ei<0 ? '\\' : '/');
//                stateEvent++;
//            }
//            prevEventIndicators[i] = ei;
//        }
//        if (stateEvent) {
//            eventUpdate(comp, &comp->eventInfo);
//            stateEvent = 0;
//        }
//#endif
//        // check for time event
//        if (comp->eventInfo.upcomingTimeEvent && comp->time > comp->eventInfo.nextEventTime) {
//            if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//                "fmiDoStep: time event detected at %g", comp->time);
//            eventUpdate(comp, &comp->eventInfo);
//        }
//
//        // terminate simulation, if requested by the model
//        if (comp->eventInfo.terminateSimulation) {
//            comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//              "fmiDoStep: model requested termination at t=%g", comp->time);
//            return fmiError; // enforce termination of the simulation loop
//        }
//    }
//    return fmiOK;
//}

//#else
// ---------------------------------------------------------------------------
// FMI functions: only for Model Exchange 1.0
// ---------------------------------------------------------------------------

//static fmiBoolean invalidNumber(fmi_Component* comp, const char* f, const char* arg, int n, int nExpected){
//    if (n != nExpected) {
//        comp->state = modelError;
//        comp->functions.logger(comp, comp->instanceName, fmiError, "error",
//                "%s: Invalid argument %s = %d. Expected %d.", f, arg, n, nExpected);
//        return fmiTrue;
//    }
//    return fmiFalse;
//}

//const char* fmiGetModelTypesPlatform() {
//    return fmiModelTypesPlatform;
//}

//fmiComponent fmiInstantiateModel(fmiString instanceName, fmiString GUID,
//        fmiCallbackFunctions functions, fmiBoolean loggingOn) {
//    return instantiateModel("fmiInstantiateModel", instanceName, GUID, functions, loggingOn);
//}

//fmiStatus ControllerImpl_fmiInitialize(fmiComponent c, fmiBoolean toleranceControlled, fmiReal relativeTolerance,
//    fmiEventInfo* eventInfo) {
//    return init("fmiInitialize", c, toleranceControlled, relativeTolerance, eventInfo);
//}

//fmiStatus ControllerImpl_fmiSetTime(fmiComponent c, fmiReal time) {
//    fmi_Component* comp = (fmi_Component *)c;
//    if (invalidState(comp, "fmiSetTime", modelInstantiated|modelInitialized))
//         return fmiError;
//    if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//            "fmiSetTime: time=%.16g", time);
//    comp->time = time;
//    return fmiOK;
//}

//fmiStatus ControllerImpl_fmiSetContinuousStates(fmiComponent c, const fmiReal x[], size_t nx){
//    fmi_Component* comp = (fmi_Component *)c;
//#if NUMBER_OF_REALS>0
//    int i;
//#endif
//    if (invalidState(comp, "fmiSetContinuousStates", modelInitialized))
//         return fmiError;
//    if (invalidNumber(comp, "fmiSetContinuousStates", "nx", nx, NUMBER_OF_STATES))
//        return fmiError;
//    if (nullPointer(comp, "fmiSetContinuousStates", "x[]", x))
//         return fmiError;
//#if NUMBER_OF_REALS>0
//    for (i=0; i<nx; i++) {
//        fmiValueReference vr = vrStates[i];
//        if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//            "fmiSetContinuousStates: #r%d#=%.16g", vr, x[i]);
//        assert(vr>=0 && vr<NUMBER_OF_REALS);
//        comp->r[vr] = x[i];
//    }
//#endif
//    return fmiOK;
//}

//fmiStatus ControllerImpl_fmiEventUpdate(fmiComponent c, fmiBoolean intermediateResults, fmiEventInfo* eventInfo) {
//    fmi_Component* comp = (fmi_Component *)c;
//    if (invalidState(comp, "fmiEventUpdate", modelInitialized))
//        return fmiError;
//    if (nullPointer(comp, "fmiEventUpdate", "eventInfo", eventInfo))
//         return fmiError;
//    if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//        "fmiEventUpdate: intermediateResults = %d", intermediateResults);
//    eventInfo->iterationConverged  = fmiTrue;
//    eventInfo->stateValueReferencesChanged = fmiFalse;
//    eventInfo->stateValuesChanged  = fmiFalse;
//    eventInfo->terminateSimulation = fmiFalse;
//    eventInfo->upcomingTimeEvent   = fmiFalse;
//    eventUpdate(comp, eventInfo); // to be implemented by the includer of this file
//    return fmiOK;
//}

//fmiStatus ControllerImpl_fmiCompletedIntegratorStep(fmiComponent c, fmiBoolean* callEventUpdate){
//    fmi_Component* comp = (fmi_Component *)c;
//    if (invalidState(comp, "fmiCompletedIntegratorStep", modelInitialized))
//         return fmiError;
//    if (nullPointer(comp, "fmiCompletedIntegratorStep", "callEventUpdate", callEventUpdate))
//         return fmiError;
//    if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//            "fmiCompletedIntegratorStep");
//    *callEventUpdate = fmiFalse;
//    return fmiOK;
//}

//fmiStatus ControllerImpl_fmiGetStateValueReferences(fmiComponent c, fmiValueReference vrx[], size_t nx){
//#if NUMBER_OF_REALS>0
//    int i;
//#endif
//    fmi_Component* comp = (fmi_Component *)c;
//    if (invalidState(comp, "fmiGetStateValueReferences", not_modelError))
//        return fmiError;
//    if (invalidNumber(comp, "fmiGetStateValueReferences", "nx", nx, NUMBER_OF_STATES))
//        return fmiError;
//    if (nullPointer(comp, "fmiGetStateValueReferences", "vrx[]", vrx))
//         return fmiError;
//#if NUMBER_OF_REALS>0
//    for (i=0; i<nx; i++) {
//        vrx[i] = vrStates[i];
//        if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//            "fmiGetStateValueReferences: vrx[%d] = %d", i, vrx[i]);
//    }
//#endif
//    return fmiOK;
//}

//fmiStatus ControllerImpl_fmiGetContinuousStates(fmiComponent c, fmiReal states[], size_t nx){
//#if NUMBER_OF_REALS>0
//    int i;
//#endif
//    fmi_Component* comp = (fmi_Component *)c;
//    if (invalidState(comp, "fmiGetContinuousStates", not_modelError))
//        return fmiError;
//    if (invalidNumber(comp, "fmiGetContinuousStates", "nx", nx, NUMBER_OF_STATES))
//        return fmiError;
//    if (nullPointer(comp, "fmiGetContinuousStates", "states[]", states))
//         return fmiError;
//#if NUMBER_OF_REALS>0
//    for (i=0; i<nx; i++) {
//        fmiValueReference vr = vrStates[i];
//        states[i] = getReal(comp, vr, nvr, value); // to be implemented by the includer of this file
//        if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//            "fmiGetContinuousStates: #r%u# = %.16g", vr, states[i]);
//    }
//#endif
//    return fmiOK;
//}

//fmiStatus ControllerImpl_fmiGetNominalContinuousStates(fmiComponent c, fmiReal x_nominal[], size_t nx){
//    int i;
//    fmi_Component* comp = (fmi_Component *)c;
//    if (invalidState(comp, "fmiGetNominalContinuousStates", not_modelError))
//        return fmiError;
//    if (invalidNumber(comp, "fmiGetNominalContinuousStates", "nx", nx, NUMBER_OF_STATES))
//        return fmiError;
//    if (nullPointer(comp, "fmiGetNominalContinuousStates", "x_nominal[]", x_nominal))
//         return fmiError;
//    x_nominal[0] = 1;
//    if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//        "fmiGetNominalContinuousStates: x_nominal[0..%d] = 1.0", nx-1);
//    for (i=0; i<nx; i++)
//        x_nominal[i] = 1;
//    return fmiOK;
//}

//fmiStatus ControllerImpl_fmiGetDerivatives(fmiComponent c, fmiReal derivatives[], size_t nx) {
//#if NUMBER_OF_STATES>0
//    int i;
//#endif
//    fmi_Component* comp = (fmi_Component *)c;
//    if (invalidState(comp, "fmiGetDerivatives", not_modelError))
//         return fmiError;
//    if (invalidNumber(c, "fmiGetDerivatives", "nx", nx, NUMBER_OF_STATES))
//        return fmiError;
//    if (nullPointer(comp, "fmiGetDerivatives", "derivatives[]", derivatives))
//         return fmiError;
//#if NUMBER_OF_STATES>0
//    for (i=0; i<nx; i++) {
//        fmiValueReference vr = vrStates[i] + 1;
//        derivatives[i] = getReal(comp, vr, nvr, value); // to be implemented by the includer of this file
//        if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//            "fmiGetDerivatives: #r%d# = %.16g", vr, derivatives[i]);
//    }
//#endif
//    return fmiOK;
//}

//fmiStatus ControllerImpl_fmiGetEventIndicators(fmiComponent c, fmiReal eventIndicators[], size_t ni) {
//#if NUMBER_OF_EVENT_INDICATORS>0
//    int i;
//#endif
//    fmi_Component* comp = (fmi_Component *)c;
//    if (invalidState(comp, "fmiGetEventIndicators", not_modelError))
//        return fmiError;
//    if (invalidNumber(comp, "fmiGetEventIndicators", "ni", ni, NUMBER_OF_EVENT_INDICATORS))
//        return fmiError;
//#if NUMBER_OF_EVENT_INDICATORS>0
//    for (i=0; i<ni; i++) {
//        eventIndicators[i] = getEventIndicator(comp, i); // to be implemented by the includer of this file
//        if (comp->loggingOn) comp->functions.logger(c, comp->instanceName, fmiOK, "log",
//            "fmiGetEventIndicators: z%d = %.16g", i, eventIndicators[i]);
//    }
//#endif
//    return fmiOK;
//}

//fmiStatus ControllerImpl_fmiTerminate(fmiComponent c){
//    return terminate("fmiTerminate", c);
//}

//void fmiFreeModelInstance(fmiComponent c) {
//    freeInstance("fmiFreeModelInstance", c);
//}
//

fmiComponent ControllerImpl_fmiInstantiateSlave(fmiString instanceName,
		fmiString fmuGUID, fmiString fmuResourceLocation,
		const fmiCallbackFunctions* functions, fmiBoolean visible,
		fmiBoolean loggingOn) {
	// create a fmiComponent and allocate storage space
	fmi_Component *c = malloc(sizeof(*c));
	if (!(conInstanceCount <= (MaxFMUInstances - 1))) {
		c->validInstance = fmiFalse;
		return c;
	} else {
		c->validInstance = fmiTrue;
		// set the name etc
		c->instanceName = instanceName;
		c->fmuGUID = fmuGUID;
		//add instance to collection;
		modelInstances[conInstanceCount] = *c;
		conInstanceCount = conInstanceCount + 1;
		return (fmiComponent) c;
	}
}

fmiStatus ControllerImpl_fmiInitializeSlave(fmiComponent c,
		fmiReal relativeTolerance, fmiReal tStart, fmiBoolean stopTimeDefined,
		fmiReal tStop) {

	fmi_Component* mc = c;
// Generated By InitialisationsListGenerator 
	mc->i[c_level_ControllerImpl_] = 100;
	mc->b[c_pumpOnReq_ControllerImpl_] = fmiFalse;
	mc->b[c_pumpOnCmd_ControllerImpl_] = fmiFalse;
	mc->b[c_warn_ControllerImpl_] = fmiFalse;
	mc->i[c_level_internal_ControllerImpl_] = 100;
	mc->b[c_pumpOnReq_internal_ControllerImpl_] = fmiFalse;
	mc->b[c_pumpOnCmd_internal_ControllerImpl_] = fmiFalse;
	mc->b[c_warn_internal_ControllerImpl_] = fmiFalse;
	mc->b[commit_ControllerImpl_] = fmiFalse;

	return fmiOK;

}

// Subroutines Generated from Events

fmiStatus ControllerImpl_fmiDoStep(fmiComponent c,
		fmiReal currentCommunicationPoint, fmiReal communicationStepSize,
		fmiBoolean noSetFMUStatePriorToCurrentPoint) {
	fmi_Component* mc = c;
// Translated code
	mc->i[c_level_internal_ControllerImpl_] = mc->i[c_level_ControllerImpl_];
	mc->b[c_pumpOnReq_internal_ControllerImpl_] =
			mc->b[c_pumpOnReq_ControllerImpl_];
	if ((mc->i[c_level_internal_ControllerImpl_] <= MIN)
			&& (mc->b[c_pumpOnReq_internal_ControllerImpl_] == fmiTrue)) {
		mc->b[c_warn_internal_ControllerImpl_] = fmiTrue;
		mc->b[commit_ControllerImpl_] = fmiTrue;
	} else {
		mc->b[c_pumpOnCmd_internal_ControllerImpl_] = fmiTrue;
		mc->b[commit_ControllerImpl_] = fmiTrue;
	}
	mc->b[c_warn_ControllerImpl_] = mc->b[c_warn_internal_ControllerImpl_];
	mc->b[c_pumpOnCmd_ControllerImpl_] =
			mc->b[c_pumpOnCmd_internal_ControllerImpl_];
	return fmiOK;
}
