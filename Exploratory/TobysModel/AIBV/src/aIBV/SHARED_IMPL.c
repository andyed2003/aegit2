#include "Common.h"
// Protected: SHARED_IMPL

// Variables and constants
int AIBV_temperature_SHARED_IMPL = NOMINAL_TEMP;
BOOL AIBV_new_temperature_measurement_SHARED_IMPL = FALSE;


// Subroutines
void SHARED_IMPL_AIBV_check_temperature_ok(int buffered_temperature)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (SHARED_IMPL)
		{
			// Check to see if guard is met
			if (((MIN_TEMP <= buffered_temperature) && (buffered_temperature <= MAX_TEMP)))
			{
				// Translated code
				AIBV_temperature_SHARED_IMPL = buffered_temperature;AIBV_new_temperature_measurement_SHARED_IMPL = TRUE;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void SHARED_IMPL_AIBV_calculate_position_partial(int *temperature)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (SHARED_IMPL)
		{
			// Check to see if guard is met
			if ((AIBV_new_temperature_measurement_SHARED_IMPL == TRUE) && ((FULLY_OPEN_TEMP <= AIBV_temperature_SHARED_IMPL) && (AIBV_temperature_SHARED_IMPL <= FULLY_CLOSED_TEMP)))
			{
				// Translated code
				AIBV_new_temperature_measurement_SHARED_IMPL = FALSE;
				(*temperature)= AIBV_temperature_SHARED_IMPL ;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void SHARED_IMPL_AIBV_calculate_position_open(int *temperature)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (SHARED_IMPL)
		{
			// Check to see if guard is met
			if ((AIBV_new_temperature_measurement_SHARED_IMPL == TRUE) && (AIBV_temperature_SHARED_IMPL < FULLY_OPEN_TEMP))
			{
				// Translated code
				AIBV_new_temperature_measurement_SHARED_IMPL = FALSE;
				(*temperature)= AIBV_temperature_SHARED_IMPL ;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void SHARED_IMPL_AIBV_calculate_position_closed(int *temperature)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (SHARED_IMPL)
		{
			// Check to see if guard is met
			if ((AIBV_new_temperature_measurement_SHARED_IMPL == TRUE) && (FULLY_CLOSED_TEMP < AIBV_temperature_SHARED_IMPL))
			{
				// Translated code
				AIBV_new_temperature_measurement_SHARED_IMPL = FALSE;
				(*temperature)= AIBV_temperature_SHARED_IMPL ;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

