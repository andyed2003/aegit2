#include "Common.h"
// EnvironTask: AIBV_plant_IMPL

// Variables and constants
int AIBV_position_AIBV_plant_IMPL = AIBV_CLOSED;
BOOL AIBV_open_started_AIBV_plant_IMPL = FALSE;
BOOL AIBV_open_performed_AIBV_plant_IMPL = FALSE;
BOOL AIBV_close_started_AIBV_plant_IMPL = FALSE;
BOOL AIBV_close_performed_AIBV_plant_IMPL = FALSE;
int AIBV_position_moving_AIBV_plant_IMPL = AIBV_CLOSED;
BOOL AIBV_measure_position_started_AIBV_plant_IMPL = FALSE;
BOOL AIBV_measure_position_performed_AIBV_plant_IMPL = FALSE;
int AIBV_new_position_measurement_AIBV_plant_IMPL = AIBV_UNKNOWN;
BOOL AIBV_undetectable_error_occured_AIBV_plant_IMPL = FALSE;
int AIBV_amount_copy_AIBV_plant_IMPL = 0;
int ENV_temperature_AIBV_plant_IMPL = NOMINAL_TEMP;
BOOL ENV_got_temperature_AIBV_plant_IMPL = FALSE;
int ENV_temperature_reading_AIBV_plant_IMPL = NOMINAL_TEMP;
const int priority_AIBV_plant_IMPL = 5;


// Subroutines
void AIBV_plant_IMPL_AIBV_open_start(int amount)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AIBV_PLANT_IMPL)
		{
			// Check to see if guard is met
			if ((AIBV_open_started_AIBV_plant_IMPL == FALSE) && (0 <= amount) && (AIBV_close_started_AIBV_plant_IMPL == FALSE))
			{
				// Translated code
				AIBV_open_started_AIBV_plant_IMPL = TRUE;
				AIBV_open_performed_AIBV_plant_IMPL = FALSE;
				AIBV_amount_copy_AIBV_plant_IMPL = amount;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AIBV_plant_IMPL_AIBV_open_end()
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AIBV_PLANT_IMPL)
		{
			// Check to see if guard is met
			if ((AIBV_open_performed_AIBV_plant_IMPL == TRUE))
			{
				// Translated code
				AIBV_position_AIBV_plant_IMPL = AIBV_position_moving_AIBV_plant_IMPL;
				AIBV_open_started_AIBV_plant_IMPL = FALSE;
				AIBV_open_performed_AIBV_plant_IMPL = FALSE;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AIBV_plant_IMPL_AIBV_close_start(int amount)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AIBV_PLANT_IMPL)
		{
			// Check to see if guard is met
			if ((AIBV_close_started_AIBV_plant_IMPL == FALSE) && (0 <= amount) && (AIBV_open_started_AIBV_plant_IMPL == FALSE))
			{
				// Translated code
				AIBV_close_started_AIBV_plant_IMPL = TRUE;
				AIBV_close_performed_AIBV_plant_IMPL = FALSE;
				AIBV_amount_copy_AIBV_plant_IMPL = amount;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AIBV_plant_IMPL_AIBV_close_end()
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AIBV_PLANT_IMPL)
		{
			// Check to see if guard is met
			if ((AIBV_close_performed_AIBV_plant_IMPL == TRUE))
			{
				// Translated code
				AIBV_position_AIBV_plant_IMPL = AIBV_position_moving_AIBV_plant_IMPL;
				AIBV_close_started_AIBV_plant_IMPL = FALSE;
				AIBV_close_performed_AIBV_plant_IMPL = FALSE;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AIBV_plant_IMPL_AIBV_measure_position_start()
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AIBV_PLANT_IMPL)
		{
			// Check to see if guard is met
			if ((AIBV_measure_position_started_AIBV_plant_IMPL == FALSE))
			{
				// Translated code
				AIBV_measure_position_started_AIBV_plant_IMPL = TRUE;
				AIBV_measure_position_performed_AIBV_plant_IMPL = FALSE;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AIBV_plant_IMPL_AIBV_measure_position_end(int *position_measurement)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AIBV_PLANT_IMPL)
		{
			// Check to see if guard is met
			if ((AIBV_measure_position_performed_AIBV_plant_IMPL == TRUE) && (((AIBV_CLOSED <= position_measurement) && (position_measurement <= AIBV_OPEN)) || (position_measurement == AIBV_UNKNOWN)))
			{
				// Translated code
				AIBV_measure_position_started_AIBV_plant_IMPL = FALSE;
				AIBV_measure_position_performed_AIBV_plant_IMPL = FALSE;
				(*position_measurement)= AIBV_new_position_measurement_AIBV_plant_IMPL ;
				
				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AIBV_plant_IMPL_AIBV_finish_control_loop()
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AIBV_PLANT_IMPL)
		{
			// Translated code
			AIBV_undetectable_error_occured_AIBV_plant_IMPL :? (((believed_position � AIBV_UNKNOWN ? believed_position � AIBV_position_AIBV_plant_IMPL) ? AIBV_undetectable_error_occured_AIBV_plant_IMPL = TRUE) ? AIBV_undetectable_error_occured_AIBV_plant_IMPL' = TRUE) ? ((�(believed_position � AIBV_UNKNOWN ? believed_position � AIBV_position_AIBV_plant_IMPL) ? AIBV_undetectable_error_occured_AIBV_plant_IMPL = FALSE) ? AIBV_undetectable_error_occured_AIBV_plant_IMPL' = FALSE);
			// [Internal] Set completed flag
			completed = TRUE;
		}
	}
}

void AIBV_plant_IMPL_AIBV_temperature_measurement(int *temperature)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AIBV_PLANT_IMPL)
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

void AIBV_plant_IMPL()
{
}
