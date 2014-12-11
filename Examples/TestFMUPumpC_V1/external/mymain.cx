/*
 * mymain.c
 *
 *  Created on: 13 Nov 2013
 *      Author: andy
 */

#include "common.h"
//#include <stdio.h>
//#include "config.h"
//#include "fmiFunctions.h"
// How many integers to set here
#define INT_NUMBER_TO_SET 1
#define BOOL_NUMBER_TO_SET 1

int main(int argc, char **argv) {

	// slave initialisation phase
	fmiReal tolerance = 0.1;
	fmiReal start = 0;
	fmiReal end = 1;
	fmiCallbackFunctions *f = malloc(sizeof(fmiCallbackFunctions));
	fmiComponent comp;
	comp = fmiInstantiateSlave("a","b","",f,fmiFalse, fmiFalse);
	fmiInitializeSlave(comp,tolerance,start,fmiTrue,end);

	// end slave initialisation

	// set-up the master's initial values:
	fmiValueReference int_vrs_toSet[INT_NUMBER_TO_SET] = {c_level_controllerImpl_};
	// the values set using the value references
	fmiInteger int_vals_toSet[INT_NUMBER_TO_SET] = {0};
	fmiValueReference bool_vrs_toSet[INT_NUMBER_TO_SET] = {c_pumpOnReq_controllerImpl_};
	// the values set using the value references
	fmiInteger bool_vals_toSet[BOOL_NUMBER_TO_SET] = {fmiTrue};

	//set the initial values for the simulation
	fmiSetInteger(comp, int_vrs_toSet, INT_NUMBER_TO_SET, int_vals_toSet );
	fmiSetBoolean(comp, bool_vrs_toSet, BOOL_NUMBER_TO_SET, bool_vals_toSet );


	// calculate some value from the environment - we use idx
	int idx = 0;
	// begin simulation phase
	for(; idx < 100; idx = idx + 1){
		int_vals_toSet[c_level_controllerImpl_] = idx;
		// update the controller with some value "environment" idx
		fmiSetInteger(comp, int_vrs_toSet, INT_NUMBER_TO_SET, int_vals_toSet );
		// do the simulation step
		fmiDoStep(comp,start,end,fmiTrue);
		// we haven't define a get integer ... but it would follow here >>>
	}
	return 0;
}
