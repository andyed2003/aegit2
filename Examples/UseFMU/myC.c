#include<stdio.h>
#include<dlfcn.h>

#include"fmiModelTypes.h"
#include"fmiModelFunctions.h"
#include "fmiModelTypes.h"
#include "myFMIDecls.h"
#define MODEL_IDENTIFIER ControllerImpl

int main(int argc, char **argv) {
	printf("hello");

	void* lib_handle;


    lib_handle = dlopen("/home/andy/git/aegit/Examples/TestFMUPumpC_V1/Release_X86_64_LINUX64/libTestFMUPumpC_V1.so", 1 );

	if (!lib_handle) {
	    fprintf(stderr, "Error during dlopen(): %s\n", dlerror());
	    exit(1);
	}

	//>>>>>>> Get the types platform
	// declare a pointer to the fmiGetTypesPlatform function
	char* (*fmiGetTypesPlatform)();
	// get the function pointer
	*(void **)(&fmiGetTypesPlatform) = dlsym(lib_handle, "ControllerImpl_fmiGetTypesPlatform");
	// declare a pointer to the return type
	const char* ret;
	// call the function and collect the return value
	ret = (*fmiGetTypesPlatform)();
	//<<<<<<<< End of: Get the types platform


//	fmiComponent fmiInstantiateSlave(fmiString instanceName, fmiString fmuGUID,
//			fmiString fmuResourceLocation, const fmiCallbackFunctions* functions,
//			fmiBoolean visible, fmiBoolean loggingOn)

	// declare a pointer to the fmiInstantiateSlave function
	fmiComponent* (*fmiInstantiateSlave)(fmiString, fmiString, fmiString, const fmiCallbackFunctions*, fmiBoolean, fmiBoolean );
//	// get the function pointer
	*(void **)(&fmiInstantiateSlave) = dlsym(lib_handle, "ControllerImpl_fmiInstantiateSlave");
	fmiComponent * ret2;
	fmiString instanceName = "thisName";
	fmiString fmuGUID = "thisGUID";
	fmiString fmuResourceLocation = "somewhere";
	fmiCallbackFunctions* functions;
	fmiBoolean visible = fmiTrue;
	fmiBoolean loggingOn = fmiFalse;
	ret2 = (*fmiInstantiateSlave) (instanceName, fmuGUID, fmuResourceLocation, functions, visible, loggingOn);


	const char* error_msg;
	error_msg = dlerror();
	if (error_msg) {
	    fprintf(stderr, "Error locating 'fmiGetTypesPlatform' - %s\n", error_msg);
	    exit(1);
	}

	dlclose(lib_handle);
	return 1;
}
