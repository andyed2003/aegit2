#ifndef COMMON_H
#define COMMON_H
typedef int BOOL;
#define TRUE 1
#define FALSE 0
void __SLEEP(int period);
#include<omp.h>
extern const int AIBV_UNKNOWN ;

extern const int AIBV_CLOSED ;

typedef enum AIBV_STATES
{
	ARMED,
	DISARMING,
	DISARMED,
} AIBV_STATES;

extern const int MAX_TEMP ;

extern const int MIN_TEMP ;

extern const int FULLY_CLOSED_TEMP ;

extern const int FULLY_OPEN_TEMP ;

extern const int NOMINAL_TEMP ;

extern const int AIBV_OPEN ;

typedef enum AIBV_ACTIONS
{
	OPEN,
	CLOSE,
	NOTHING,
} AIBV_ACTIONS;

#include "AAAA_SHARED_IMPL.h"
#include "AAAA_CONTROL_LOOP_IMPL.h"
#include "AAAA_TEMPERATURE_IMPL.h"
#include "AIBV_plant_IMPL.h"
#endif

