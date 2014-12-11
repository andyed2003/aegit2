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
#define NUMBER_TO_SET 1

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
	fmiValueReference vrs_toSet[NUMBER_TO_SET] = {a_controllerImpl_};
	// the values set using the value references
	fmiInteger vals_toSet[NUMBER_TO_SET] = {99};
	// calculate some value from the environment - we use idx
	int idx = 100;
	// begin simulation phase
	for(; idx < 1000; idx = idx + 100){
		vals_toSet[0] = idx;
		// update the controller with some value "environment" idx
		fmiSetInteger(comp, vrs_toSet, NUMBER_TO_SET, vals_toSet );
		// do the simulation step
		fmiDoStep(comp,start,end,fmiTrue);
		// we haven't define a get integer ... but it would follow here >>>
	}
	return 0;
}
