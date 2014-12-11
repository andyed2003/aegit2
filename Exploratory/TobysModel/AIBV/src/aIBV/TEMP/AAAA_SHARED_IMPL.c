#include "Common.h"
// Protected: AAAA_SHARED_IMPL

// Variables and constants
int AIBV_temperature_AAAA_SHARED_IMPL = NOMINAL_TEMP;
BOOL AIBV_new_temperature_measurement_AAAA_SHARED_IMPL = FALSE;


// Subroutines
void AAAA_SHARED_IMPL_AIBV_check_temperature_ok(int buffered_temperature)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AAAA_SHARED_IMPL)
		{
			// Check to see if guard is met
			if (((MIN_TEMP <= buffered_temperature) && (buffered_temperature <= MAX_TEMP)))
			{
				// Translated code
				AIBV_temperature_AAAA_SHARED_IMPL = buffered_temperature;AIBV_new_temperature_measurement_AAAA_SHARED_IMPL = TRUE;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AAAA_SHARED_IMPL_AIBV_get_temperature(int *temperature, BOOL *new_temperature_measurement)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AAAA_SHARED_IMPL)
		{
			// Check to see if guard is met
			if ((AIBV_new_temperature_measurement_AAAA_SHARED_IMPL == TRUE))
			{
				// Translated code
				(*temperature)= AIBV_temperature_AAAA_SHARED_IMPL ;
				(*new_temperature_measurement)= AIBV_new_temperature_measurement_AAAA_SHARED_IMPL ;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AAAA_SHARED_IMPL_AIBV_set_new_temperature_measurement(BOOL new_temperature_measurement)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AAAA_SHARED_IMPL)
		{
			// Translated code
			AIBV_new_temperature_measurement_AAAA_SHARED_IMPL = new_temperature_measurement;
			// [Internal] Set completed flag
			completed = TRUE;
		}
	}
}

