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
BOOL AIBV_position_error_light_on = FALSE;
const int priority_AIBV_plant_IMPL = 5;


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

AIBV_ACTIONS TEST_action = NOTHING;

typedef enum ACTUATE_ERRORS
{
	ACT_SUCCEED,
	ACT_IGNORE,
	ACT_OVERDRIVE,
	ACT_RANDOM,
	ACT_OPPOSITE,
	ACT_HIDE,
} ACTUATE_ERRORS;

typedef enum MEASURE_ERRORS
{
	MEAS_SUCCEED,
	MEAS_RANDOM,
	MEAS_TIMEOUT,
	MEAS_HIDE,
} MEASURE_ERRORS;

typedef struct Test
{
	int            temperature;
	int            target_position;
	AIBV_ACTIONS   action;
	int            amount;
	ACTUATE_ERRORS actuate_result;
	MEASURE_ERRORS measure_result;
	BOOL           expect_error;
} Test;

Test TEST_tests[] =
{
	{  0, 99, OPEN,    99, ACT_SUCCEED,   MEAS_SUCCEED, FALSE},
	{200,  0, CLOSE,   99, ACT_SUCCEED,   MEAS_SUCCEED, FALSE},
	{  0, 99, OPEN,    99, ACT_HIDE,      MEAS_HIDE,    FALSE},
	{  0, 99, NOTHING,  0, ACT_SUCCEED,   MEAS_SUCCEED, FALSE},
	{200,  0, CLOSE,   99, ACT_OVERDRIVE, MEAS_SUCCEED, FALSE},
	{100, 33, OPEN,    33, ACT_HIDE,      MEAS_HIDE,    FALSE},
	{ 50, 88, OPEN,    55, ACT_SUCCEED,   MEAS_SUCCEED, TRUE },
	{ 40, 99, NOTHING,  0, ACT_SUCCEED,   MEAS_SUCCEED, FALSE},
};

int TEST_num_tests = 8;
int TEST_current_test = -1;
int TEST_num_control_loops_done = 0;
int TEST_test_results_printed = 0;
// STJW:
// The number of tests should be computed from the size of the array (somehow!).

void TEST_actuate()
{
	if (TEST_action == OPEN)
	{
		if (TEST_tests[TEST_current_test].action == OPEN)
		{
			switch (TEST_tests[TEST_current_test].actuate_result)
			{
				case ACT_SUCCEED:
					AIBV_position_moving_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL + AIBV_amount_copy_AIBV_plant_IMPL;
					break;
				case ACT_IGNORE:
					AIBV_position_moving_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL;
					break;
				case ACT_OVERDRIVE:
					AIBV_position_moving_AIBV_plant_IMPL = AIBV_OPEN;
					break;
				case ACT_RANDOM:
					AIBV_position_moving_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL + (rand() % (AIBV_amount_copy_AIBV_plant_IMPL - 1) + 1);
					break;
				case ACT_OPPOSITE:
					AIBV_position_moving_AIBV_plant_IMPL = MAX(AIBV_position_AIBV_plant_IMPL - AIBV_amount_copy_AIBV_plant_IMPL, AIBV_CLOSED);
					break;
				case ACT_HIDE:
					AIBV_position_moving_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL + AIBV_amount_copy_AIBV_plant_IMPL - 1;
					break;
			}
		}
		else
		{
			printf("ERROR: controller performing unwanted open\n");
		}
		AIBV_open_performed_AIBV_plant_IMPL = TRUE;
	}
	else if (TEST_action == CLOSE && TEST_tests[TEST_current_test].action == CLOSE)
	{
		if (TEST_tests[TEST_current_test].action == CLOSE)
		{
			switch (TEST_tests[TEST_current_test].actuate_result)
			{
				case ACT_SUCCEED:
					AIBV_position_moving_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL - AIBV_amount_copy_AIBV_plant_IMPL;
					break;
				case ACT_IGNORE:
					AIBV_position_moving_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL;
					break;
				case ACT_OVERDRIVE:
					AIBV_position_moving_AIBV_plant_IMPL = AIBV_CLOSED;
					break;
				case ACT_RANDOM:
					AIBV_position_moving_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL - (rand() % (AIBV_amount_copy_AIBV_plant_IMPL - 1) + 1);
					break;
				case ACT_OPPOSITE:
					AIBV_position_moving_AIBV_plant_IMPL = MIN(AIBV_position_AIBV_plant_IMPL + AIBV_amount_copy_AIBV_plant_IMPL, AIBV_OPEN);
					break;
				case ACT_HIDE:
					AIBV_position_moving_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL - AIBV_amount_copy_AIBV_plant_IMPL + 1;
					break;
			}
		}
		else
		{
			printf("ERROR: controller performing unwanted close\n");
		}
		AIBV_close_performed_AIBV_plant_IMPL = TRUE;
	}
	//__SLEEP(100 * AIBV_amount_copy_AIBV_plant_IMPL);
}

void TEST_measure()
{
	switch (TEST_tests[TEST_current_test].measure_result)
	{
		case MEAS_SUCCEED:
			AIBV_new_position_measurement_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL;
			break;
		case MEAS_RANDOM:
			AIBV_new_position_measurement_AIBV_plant_IMPL = rand() % (AIBV_OPEN - AIBV_CLOSED + 1);
			break;
		case MEAS_TIMEOUT:
			AIBV_new_position_measurement_AIBV_plant_IMPL = AIBV_UNKNOWN;
			break;
		case MEAS_HIDE:
			switch (TEST_tests[TEST_current_test].action)
			{
				case OPEN:
					AIBV_new_position_measurement_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL + 1;
					break;
				case CLOSE:
					AIBV_new_position_measurement_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL - 1;
					break;
				case NOTHING:
					AIBV_new_position_measurement_AIBV_plant_IMPL = AIBV_position_AIBV_plant_IMPL;
					break;
			}
			break;
	}
	AIBV_measure_position_performed_AIBV_plant_IMPL = TRUE;
	//__SLEEP(10);
}

void TEST_loop_finished()
{
	printf("  Control loop finished:");
	switch (TEST_action)
	{
		case OPEN:
			printf(" %7s", "OPEN");
			break;
		case CLOSE:
			printf(" %7s", "CLOSE");
			break;
		case NOTHING:
			printf(" %7s", "NOTHING");
			break;
	}
	printf(", %3d", AIBV_amount_copy_AIBV_plant_IMPL);
	printf(", %3d", AIBV_position_AIBV_plant_IMPL);
	printf(", %3d", AIBV_new_position_measurement_AIBV_plant_IMPL);
	switch (AIBV_position_error_light_on)
	{
		case TRUE:
			printf(", %5s", "TRUE");
			break;
		case FALSE:
			printf(", %5s", "FALSE");
			break;
	}
	if (TEST_tests[TEST_current_test].action == TEST_action &&
            TEST_tests[TEST_current_test].amount == AIBV_amount_copy_AIBV_plant_IMPL &&
            TEST_tests[TEST_current_test].expect_error == AIBV_position_error_light_on)
	{
		printf(", %30s", "PASS");
	}
	else
	{
		printf(", %30s", "FAIL");
	}
	printf("\n\n");

	TEST_action = NOTHING;
	TEST_num_control_loops_done++;
	TEST_test_results_printed++;
}

void TEST_next()
{
	if (TEST_action == NOTHING)
	{
		if (TEST_current_test + 1 > TEST_test_results_printed)
		{
			if (TEST_tests[TEST_current_test].action == NOTHING)
			{
				printf("  No control action performed:%56s\n\n", "PASS");
			}
			else
			{
				printf("  No control action performed:%56s\n\n", "FAIL");
			}
			TEST_test_results_printed++;
		}

		if (TEST_current_test < TEST_num_tests - 1)
		{
			TEST_current_test++;
			ENV_temperature_reading_AIBV_plant_IMPL = TEST_tests[TEST_current_test].temperature;
			ENV_got_temperature_AIBV_plant_IMPL = TRUE;

			printf("Starting test: %6.1f", TEST_tests[TEST_current_test].temperature / 10.0);
			printf(", %3d", TEST_tests[TEST_current_test].target_position);
			switch (TEST_tests[TEST_current_test].action)
			{
				case OPEN:
					printf(", %7s", "OPEN");
					break;
				case CLOSE:
					printf(", %7s", "CLOSE");
					break;
				case NOTHING:
					printf(", %7s", "NOTHING");
					break;
			}
			printf(", %3d", TEST_tests[TEST_current_test].amount);
			switch (TEST_tests[TEST_current_test].actuate_result)
			{
				case ACT_SUCCEED:
					printf(", %13s", "ACT_SUCCEED");
					break;
				case ACT_IGNORE:
					printf(", %13s", "ACT_IGNORE");
					break;
				case ACT_OVERDRIVE:
					printf(", %13s", "ACT_OVERDRIVE");
					break;
				case ACT_RANDOM:
					printf(", %13s", "ACT_RANDOM");
					break;
				case ACT_OPPOSITE:
					printf(", %13s", "ACT_OPPOSITE");
					break;
				case ACT_HIDE:
					printf(", %13s", "ACT_HIDE");
					break;
			}
			switch (TEST_tests[TEST_current_test].measure_result)
			{
				case MEAS_SUCCEED:
					printf(", %12s", "MEAS_SUCCEED");
					break;
      				case MEAS_RANDOM:
					printf(", %12s", "MEAS_RANDOM");
					break;
       				case MEAS_TIMEOUT:
					printf(", %12s", "MEAS_TIMEOUT");
					break;
				case MEAS_HIDE:
					printf(", %12s", "MEAS_HIDE");
					break;
			}
			switch (TEST_tests[TEST_current_test].expect_error)
			{
				case TRUE:
					printf(", %5s", "TRUE");
					break;
				case FALSE:
					printf(", %5s", "FALSE");
					break;
			}
			printf("\n");
		}
		else
		{
			printf("\nNumber of control loops not performed: %47d\n", TEST_num_tests - TEST_num_control_loops_done);

			if (TEST_num_tests != TEST_test_results_printed)
			{
				printf("\nERROR: number of printed test results does not equal the number of tests performed.\n");
			}

			exit(0);
		}
	}
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

void AIBV_plant_IMPL_AIBV_temperature_measurement(int *temperature)
{
	// [Internal] This will block until the guard has been met (no nice way of doing this in OpenMP 2.5)
	BOOL completed = FALSE;          // [Internal] Private to the thread by default
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
					TEST_next();

					// [Internal] Set completed flag
					completed = TRUE;
				}
			}
		}
		// STJW:
		// The above locking behaviour is needed as to prevent this thread simultaneously trying to start a new test as the controller is finishing the previous test (race on TEST_action).
		// This is not very nice, and limits the ability to feed the controller new temperature values whilst it is performing a control action. Need to change this.
		// Adding a "started test" queue will not always help as the controller will only remember the most recent pending temperature reading received during a control action.
		// If we make the actuation duration realistic (make the controller sleep), or do the same for measurement, then the above race condition becomes hard to manage without the lock.
		// Also, how do we know the controller has actually finished a control action (rather than not being in one of our methods)?

		// STJW:
		// Probably ought to write the test harness in Event-B and then generate the code!

                // [Internal] Code to monitor time between periodic tasks
                internalTimeDifference = omp_get_wtime() - internalPeriodicStartTime;
                __SLEEP(500 - (internalTimeDifference * 1000));
	}
}
