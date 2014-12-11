#include "Common.h"
#include "fakeFMIDecls.h"

#include <stdio.h>
#include <stdlib.h>

fmiComponent *modelInstances[MaxControllerModels]; // start of with an empty array of components
int conInstanceCount = 0;

// Subroutines
void controllerImpl_controllerSMstateMachine(fmiComponent *c) {
	// get the level
	fmiInteger level = c->i[c_Level_controllerImpl_];

	switch (c->SM_ProgramCounter) {
	case ready:
		if (level < Low) {
			c->i[c_time_controllerImpl_] = ((c->i[c_time_controllerImpl_]) + ComputeDelay);
			c->b[c_pumpOn_controllerImpl_] = fmiTrue;
		} else if (level > High) {
			c->b[c_pumpOn_controllerImpl_] = fmiFalse;
			c->i[c_time_controllerImpl_] = ((c->i[c_time_controllerImpl_]) + ComputeDelay);
		} else if (level >= Low
				&& level <= High) {
			c->i[c_time_controllerImpl_] = ((c->i[c_time_controllerImpl_]) + ComputeDelay);
		}
		break;
	}

}

// a minimal constructor
fmiComponent *fmiInstantiateControllerImpl(fmiString instanceName,
		fmiString GUID) {
	// create a fmiComponent and allocate storage space
	fmiComponent *newFMIComponent = malloc(sizeof(*newFMIComponent));
	if (!(conInstanceCount <= (MaxControllerModels - 1))) {
		newFMIComponent->validInstance = fmiFalse;
		return newFMIComponent;
	} else {
		newFMIComponent->validInstance = fmiTrue;
		// set the name etc
		newFMIComponent->fmuInstanceName = instanceName;
		newFMIComponent->fmuGUID = GUID;
		newFMIComponent->i[c_Level_controllerImpl_] = 0; //initialise variable c_Level_controllerImpl
		newFMIComponent->i[c_time_controllerImpl_] = 0; //initialise variable c_time_controllerImpl
		newFMIComponent->b[c_pumpOn_controllerImpl_] = fmiFalse; //initialise variable c_pumpOn_controllerImpl_
		newFMIComponent->SM_ProgramCounter = ready; // set the initial program counter position
		//add instance to collection;
		modelInstances[conInstanceCount] = newFMIComponent;
		conInstanceCount = conInstanceCount + 1;
		return newFMIComponent;
	}
}

// The master uses this function to get the controller's decision
// about the pump command.
void controller_fmiGetBoolean(fmiComponent *c,
		const fmiValueReference vr[c_booleanArraySize], size_t nvr,
		fmiBoolean value[c_booleanArraySize]) {
	value[c_pumpOn_controllerImpl_] = c->b[vr[c_pumpOn_controllerImpl_]];

	printf("return value c_pumpOn... : %i\n", value[c_pumpOn_controllerImpl_]);

}

// The master communicates the integer value of the level,
// to the controller, passed from the environment
void controller_fmiSetInteger(fmiComponent *c,
		const fmiValueReference vr[c_integerArraySize], size_t nvr,
		fmiInteger value[c_integerArraySize]) {
	c->i[vr[c_Level_controllerImpl_]] = value[c_Level_controllerImpl_];
	c->i[vr[c_time_controllerImpl_]] = value[c_time_controllerImpl_];

	printf("%u \t: controller_fmiSetInteger set value c_Level_controllerImpl_... \n",
			value[c_Level_controllerImpl_]);
	printf("%u \t: controller_fmiSetInteger set value c_time_controllerImpl_ ... \n",
			value[c_time_controllerImpl_]);

}

// the simulation step.
void controller_fmiDoStep(fmiComponent *c) {

	// Translated code
	controllerImpl_controllerSMstateMachine(c);
	printf("%i \t : controller_fmiDoStep reports c_PumpOn \n", c->b[c_pumpOn_controllerImpl_]);
}
