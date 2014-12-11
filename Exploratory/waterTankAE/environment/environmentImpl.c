#include "Common.h"
#include "fakeFMIDecls.h"

#include <stdio.h>
#include <stdlib.h>

int envInstanceCount = 0;
fmiComponent *modelInstances[MaxEnviromentModels];

// Subroutines
void environmentImpl_environSMstateMachine(fmiComponent *c) {
	// Get the pumpOn value
	fmiBoolean pumpOn = c->b[env_pumpOn_environmentImpl_];

	switch (c->SM_ProgramCounter) {
	case stableSim:
		if (pumpOn == fmiFalse) {
			c->i[env_Level_environmentImpl_] = ((c->i[env_Level_environmentImpl_]) - 1);
		} else if (pumpOn == fmiTrue) {
			c->i[env_Level_environmentImpl_] = ((c->i[env_Level_environmentImpl_]) + 1);
		}
		break;
	}
}

// a minimal constructor
fmiComponent *fmiInstantiateEnvironmentImplImpl(fmiString instanceName,
		fmiString GUID) {
	// create a fmiComponent and allocate storage space
	fmiComponent *newFMIComponent = malloc(sizeof(*newFMIComponent));
	if (!(envInstanceCount <= (MaxEnviromentModels - 1))) {
		newFMIComponent->validInstance = fmiFalse;
		return newFMIComponent;
	} else {
		newFMIComponent->validInstance = fmiTrue;
		// set the name etc
		newFMIComponent->fmuInstanceName = instanceName;
		newFMIComponent->fmuGUID = GUID;
		newFMIComponent->i[env_Level_environmentImpl_] = 1500; //initialise variable fmi_Level_environmentImpl
		newFMIComponent->b[env_pumpOn_environmentImpl_] = fmiFalse; //initialise variable fmi_pumpOn_environmentImpl
		newFMIComponent->SM_ProgramCounter = stableSim; // set the initial program counter position

		//add instance to collection;
		modelInstances[envInstanceCount] = newFMIComponent;
		envInstanceCount = envInstanceCount + 1;
		return newFMIComponent;
	}
}

// The master gets the level from the environment using this function
void environmentImpl_fmiGetInteger(fmiComponent *c,
		const fmiValueReference vr[env_integerArraySize], size_t nvr,
		fmiInteger value[env_integerArraySize]) {
	value[env_Level_environmentImpl_] = c->i[env_Level_environmentImpl_];

	printf("%i \t: environmentImpl_fmiGetInteger returned value f_Level_environmentImpl_... \n",
			value[env_Level_environmentImpl_]);
}

// The master sets the pump on command from the controller using this
void environmentImpl_fmiSetBoolean(fmiComponent *c,
		const fmiValueReference vr[env_booleanArraySize], size_t nvr, fmiInteger value[env_booleanArraySize]) {
	c->b[vr[env_pumpOn_environmentImpl_]] = value[env_pumpOn_environmentImpl_];

	printf("%i \t: environmentImpl_fmiSetBoolean set value fmi_pumpOn... \n", value[env_pumpOn_environmentImpl_]);

}

// The simulation step
void environmentImpl_fmiDoStep(fmiComponent *c) {
	environmentImpl_environSMstateMachine(c);
	printf("%i \t: environmentImpl_fmiDoStep reports fmiLevel \n", c->i[env_Level_environmentImpl_]);
	printf("%i \t: environmentImpl_fmiDoStep reports fmi_PumpOn \n", c->b[env_pumpOn_environmentImpl_]);
}
