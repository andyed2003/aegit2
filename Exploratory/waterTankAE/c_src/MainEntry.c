// MainEntry: MainEntry
#include "Common.h"
#include "controllerImpl.h"
#include "environmentImpl.h"
#include "fakeFMIDecls.h"
// Global defns
int main(int argc, char *argv[]) {
	fmiString c_name = "controllerImpl";
	fmiString c_guid = "007";
	fmiString env_name = "envronmentImpl";
	fmiString env_guid = "008";

	fmiInteger c_time = 0;

	fmiComponent *conComp = fmiInstantiateControllerImpl(c_name, c_guid);
	fmiComponent *envComp = fmiInstantiateEnvironmentImplImpl(env_name, env_guid);

	if(conComp->validInstance == fmiFalse){
		printf("Too many FMU's: %s \n", conComp->fmuInstanceName);
		return -1;
	}

	if(envComp->validInstance == fmiFalse){
		printf("Too many FMU's: %s \n", envComp->fmuInstanceName);
		return -1;
	}

	// master getting/receiving values from the environment FMU
	fmiValueReference envNamedIntegerValuesToGet[env_integerArraySize] = {env_Level_environmentImpl_};
	fmiInteger envRecievedIntegerValues[env_integerArraySize];

	// master getting/receiving from the controller FMU
	fmiValueReference conNamedBooleanValuesToGet[c_booleanArraySize] = {c_pumpOn_controllerImpl_};
	fmiBoolean conRecievedBooleanValues[c_booleanArraySize];

	// master sending values to both FMUs
	fmiInteger conNamedIntegerValuesToSet[c_integerArraySize] = {c_Level_controllerImpl_, c_time_controllerImpl_};
	fmiInteger conIntegerValuesToSet[c_integerArraySize];
	fmiBoolean envBooleanValuesToSet[env_booleanArraySize] = {env_pumpOn_environmentImpl_};

	// I am the master! Tell me about your variables 0-0`
	int i = 0;
	for (i = 0; i < MasterSteps; i = i + 1) {

		conIntegerValuesToSet[c_time_controllerImpl_] = c_time;
		c_time =  c_time + 1;

		controller_fmiDoStep(conComp);
		environmentImpl_fmiDoStep(envComp);

		// update master:
		// get integer fmi_Level from environment
		environmentImpl_fmiGetInteger(envComp, envNamedIntegerValuesToGet, sizeof(envNamedIntegerValuesToGet), envRecievedIntegerValues);

		// The envRecievedIntegers have to have the time added. So we manually
		// transfer data to conIntegersValuesToSet, adding the additional time vale that we require
		// as we proceed through the loop below
		conIntegerValuesToSet[c_Level_controllerImpl_] = envRecievedIntegerValues[env_Level_environmentImpl_];

		// update master:
		// get Boolean c_PumpOn from controller
		printf("getting bool ref: %i\n", conNamedBooleanValuesToGet[0]);
		controller_fmiGetBoolean(conComp, conNamedBooleanValuesToGet, sizeof(conNamedBooleanValuesToGet) ,conRecievedBooleanValues);

		// update controller:
		// set c_Level to fmi_Level from environment, and time from master
		controller_fmiSetInteger(conComp, conNamedIntegerValuesToSet, sizeof(conNamedIntegerValuesToSet), conIntegerValuesToSet );

		// update environment:
		// set fmi_PumpOn to c_PumpOn from controller
		environmentImpl_fmiSetBoolean(envComp, envBooleanValuesToSet, sizeof(envBooleanValuesToSet), conRecievedBooleanValues );
	}
	return 0;
}

