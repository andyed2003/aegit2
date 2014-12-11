#ifndef CONTROLLERIMPL_H
#define CONTROLLERIMPL_H
#define MODEL_IDENTIFIER ControllerImpl
static fmiValueReference c_level_ControllerImpl_ = 0;
static fmiValueReference c_pumpOnReq_ControllerImpl_ = 0;
static fmiValueReference c_pumpOnCmd_ControllerImpl_ = 1;
static fmiValueReference c_warn_ControllerImpl_ = 2;
static fmiValueReference c_level_internal_ControllerImpl_ = 1;
static fmiValueReference c_pumpOnReq_internal_ControllerImpl_ = 3;
static fmiValueReference c_pumpOnCmd_internal_ControllerImpl_ = 4;
static fmiValueReference c_warn_internal_ControllerImpl_ = 5;
static fmiValueReference commit_ControllerImpl_ = 6;
fmiComponent fmiInstantiateSlave(fmiString instanceName, fmiString fmuGUID,
		fmiString fmuResourceLocation, const fmiCallbackFunctions* functions,
		fmiBoolean visible, fmiBoolean loggingOn);
fmiStatus fmiInitializeSlave(fmiComponent c, fmiReal tStart,
		fmiBoolean StopTimeDefined, fmiReal tStop);
fmiStatus fmiDoStep(fmiComponent c, fmiReal currentCommunicationPoint,
		fmiReal communicationStepSize,
		fmiBoolean noSetFMUStatePriorToCurrentPoint);
#endif

