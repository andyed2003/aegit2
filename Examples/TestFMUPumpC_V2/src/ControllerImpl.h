#ifndef CONTROLLERIMPL_H
#define CONTROLLERIMPL_H
static fmiValueReference c_level_ControllerImpl_ = 0;
static fmiValueReference c_pumpOnReq_ControllerImpl_ = 0;
static fmiValueReference c_pumpOnCmd_ControllerImpl_ = 1;
static fmiValueReference c_warn_ControllerImpl_ = 2;
static fmiValueReference c_level_internal_ControllerImpl_ = 1;
static fmiValueReference c_pumpOnReq_internal_ControllerImpl_ = 3;
static fmiValueReference c_pumpOnCmd_internal_ControllerImpl_ = 4;
static fmiValueReference c_warn_internal_ControllerImpl_ = 5;
static fmiValueReference commit_ControllerImpl_ = 6;
fmiComponent ControllerImpl_fmiInstantiateSlave(fmiString instanceName,
		fmiString fmuGUID, fmiString fmuResourceLocation,
		const fmiCallbackFunctions* functions, fmiBoolean visible,
		fmiBoolean loggingOn);
fmiStatus ControllerImpl_fmiInitializeSlave(fmiComponent c,
		fmiReal relativeTolerance, fmiReal tStart, fmiBoolean stopTimeDefined,
		fmiReal tStop);
fmiStatus ControllerImpl_fmiDoStep(fmiComponent c,
		fmiReal currentCommunicationPoint, fmiReal communicationStepSize,
		fmiBoolean noSetFMUStatePriorToCurrentPoint);
#endif

