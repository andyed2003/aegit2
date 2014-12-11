#include "Common.h"
// EnvironTask: AIBV_plant_IMPL

// Variables and constants
int ENV_temperature_AIBV_plant_IMPL = NOMINAL_TEMP;
BOOL ENV_got_temperature_AIBV_plant_IMPL = FALSE;
int ENV_temperature_reading_AIBV_plant_IMPL = NOMINAL_TEMP;
const int priority_AIBV_plant_IMPL2 = 5;


// Subroutines
void AIBV_plant_IMPL_AIBV_temperature_measurement(int *temperature)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AIBV_PLANT_IMPL2)
		{
			// Check to see if guard is met
			if ((ENV_got_temperature_AIBV_plant_IMPL == TRUE))
			{
				// Translated code
				ENV_got_temperature_AIBV_plant_IMPL = FALSE;
				(*temperature)= ENV_temperature_reading_AIBV_plant_IMPL ;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AIBV_plant_IMPL2()
{
	while(TRUE)
	{
                // [Internal] Timer information for repeating or periodic tasks
                double internalPeriodicStartTime = omp_get_wtime();
                double internalTimeDifference;

		ENV_temperature_reading_AIBV_plant_IMPL--;
		ENV_got_temperature_AIBV_plant_IMPL = TRUE;
		printf("ENV_temperature_reading = %f\n", ENV_temperature_reading_AIBV_plant_IMPL / 10.0);

                // [Internal] Code to monitor time between periodic tasks
                internalTimeDifference = omp_get_wtime() - internalPeriodicStartTime;
                __SLEEP(500 - (internalTimeDifference * 1000));
	}
}
