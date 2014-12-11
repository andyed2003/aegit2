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
BOOL AIBV_position_error_light_on = FALSE;
const int priority_AIBV_plant_IMPL = 5;


AIBV_ACTIONS TEST_action = NOTHING;


void TEST_actuate()
{
	if (TEST_action == OPEN)
	{
		AIBV_position_moving_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL + AIBV_amount_copy_AIBV_plant_IMPL;
		AIBV_open_performed_AIBV_plant_IMPL = TRUE;
	}
	else if (TEST_action == CLOSE)
	{
		AIBV_position_moving_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL - AIBV_amount_copy_AIBV_plant_IMPL;
		AIBV_close_performed_AIBV_plant_IMPL = TRUE;
	}
	__SLEEP(500 * AIBV_amount_copy_AIBV_plant_IMPL);
}

void TEST_measure()
{
	AIBV_measure_position_performed_AIBV_plant_IMPL = TRUE;
	AIBV_new_position_measurement_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL;
}

void TEST_loop_finished()
{
	printf("Control loop finished:\n");
	if (TEST_action == OPEN)
	{
		printf("  Action         = OPEN\n");
	}
	else if (TEST_action == CLOSE)
	{
		printf("  Action         = CLOSE\n");
	}
	else
	{
		printf("  Action         = NOTHING\n");
	}
	printf("  amount         = %d\n", AIBV_amount_copy_AIBV_plant_IMPL);
	printf("  final position = %d\n", AIBV_position_AIBV_plant_IMPL);
	if (AIBV_position_error_light_on == FALSE)
	{
		printf("  error flag     = FALSE\n");
	}
	else
	{
		printf("  error flag     = TRUE\n");
	}
	TEST_action = NOTHING;
}


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
				
				TEST_action = OPEN;
				TEST_actuate();

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
				
				TEST_action = CLOSE;
				TEST_actuate();

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
				
				TEST_measure();

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
			if ((AIBV_measure_position_performed_AIBV_plant_IMPL == TRUE) && (((AIBV_CLOSED <= AIBV_new_position_measurement_AIBV_plant_IMPL) && (AIBV_new_position_measurement_AIBV_plant_IMPL <= AIBV_OPEN)) || (AIBV_new_position_measurement_AIBV_plant_IMPL == AIBV_UNKNOWN)))
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
			// Check to see if guard is met
			if (TRUE)
			{
				// Translated code

				TEST_loop_finished();

				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AIBV_plant_IMPL_AIBV_position_error_light_on()
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE; 	 // [Internal] Private to the thread by default
	while(!completed)
	{
		// [Internal] Try and get lock
		#pragma omp critical (AIBV_PLANT_IMPL)
		{
			// Check to see if guard is met
			if (TRUE)
			{
				// Translated code
				AIBV_position_error_light_on = TRUE;

				// [Internal] Set completed flag
				completed = TRUE;
			}
		}
	}
}

void AIBV_plant_IMPL()
{
	while(TRUE)
	{
                // [Internal] Timer information for repeating or periodic tasks
                double internalPeriodicStartTime = omp_get_wtime();
                double internalTimeDifference;

                // [Internal] Code to monitor time between periodic tasks
                internalTimeDifference = omp_get_wtime() - internalPeriodicStartTime;
                __SLEEP(500 - (internalTimeDifference * 1000));
	}
}
