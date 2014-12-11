#ifndef COMMON_H
#define COMMON_H
typedef int BOOL;
#define TRUE 1
#define FALSE 0
void __SLEEP(int period);
#include<omp.h>
#include<stdio.h>
#define AIBV_UNKNOWN -1

#define AIBV_CLOSED 0

typedef enum AIBV_STATES
{
	ARMED,
	DISARMING,
	DISARMED,
} AIBV_STATES;

#define MAX_TEMP 600

#define MIN_TEMP -400

#define FULLY_CLOSED_TEMP 130

#define FULLY_OPEN_TEMP 40

#define NOMINAL_TEMP 200

#define AIBV_OPEN 99

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

