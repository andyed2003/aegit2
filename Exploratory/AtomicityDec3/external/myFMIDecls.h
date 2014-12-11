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

// fmiComponent
typedef struct{
	fmiBoolean validInstance;
	fmiString instanceName;
	fmiString fmuGUID;
    fmiCallbackFunctions functions;
    ModelState state;
    fmiBoolean loggingOn;
	fmiValueReference i[NUMBER_OF_INTEGERS]; // my array of fmiValueReferences to integers
	fmiValueReference b[NUMBER_OF_BOOLEANS]; // my array of fmiValueReferences to booleans
	fmiValueReference r[NUMBER_OF_REALS]; // my array of fmiValueReferences to real
	fmiValueReference s[NUMBER_OF_STRINGS]; // my array of fmiValueReferences to strings
#ifdef FMI_COSIMULATION
    fmiEventInfo eventInfo;
#endif
} fmi_Component;


#endif /* myFMIDECLS_H_ */
