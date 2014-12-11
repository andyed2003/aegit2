/*
 * myFMIDecls.h
 *
 *  Created on: 29 Jul 2013
 *      Author: andy
 */
#include "config.h"

#ifndef myFMIDECLS_H_
#define myFMIDECLS_H_

#define not_modelError (modelInstantiated|modelInitialized|modelTerminated)

typedef enum {
    modelInstantiated = 1<<0,
    modelInitialized  = 1<<1,
    modelTerminated   = 1<<2,
    modelError        = 1<<3
} ModelState;

typedef enum {fmiDoStepStatus,
	fmiPendingStatus,
	fmiLastSuccessfulTime,
} fmiStatusKind;

//// fmiComponent
typedef struct{
	fmiBoolean validInstance;
	fmiString instanceName;
	fmiString GUID;
    fmiCallbackFunctions functions;
    fmiReal time;
    ModelState state;
    fmiBoolean loggingOn;
	fmiInteger i[NUMBER_OF_INTEGERS]; // my array of fmiValueReferences to integers
	fmiBoolean b[NUMBER_OF_BOOLEANS]; // my array of fmiValueReferences to booleans
	fmiReal r[NUMBER_OF_REALS]; // my array of fmiValueReferences to real
	fmiString s[NUMBER_OF_STRINGS]; // my array of fmiValueReferences to strings
#ifdef FMI_COSIMULATION
    fmiEventInfo eventInfo;
#endif
} ModelInstance;


#endif /* myFMIDECLS_H_ */
