#include "Common.h"
// Task: AAAA_CONTROL_LOOP_IMPL

// Variables and constants
//AIBV_STATES AIBV_state_AAAA_CONTROL_LOOP_IMPL = DISARMED;
AIBV_STATES AIBV_state_AAAA_CONTROL_LOOP_IMPL = ARMED;
AIBV_ACTIONS AIBV_action_AAAA_CONTROL_LOOP_IMPL = NOTHING;
int AIBV_amount_AAAA_CONTROL_LOOP_IMPL = 0;
BOOL AIBV_control_action_started_AAAA_CONTROL_LOOP_IMPL = FALSE;
BOOL AIBV_action_needs_to_measure_position_AAAA_CONTROL_LOOP_IMPL = FALSE;
int AIBV_measured_position_AAAA_CONTROL_LOOP_IMPL = AIBV_UNKNOWN;
BOOL AIBV_position_measurement_to_check_AAAA_CONTROL_LOOP_IMPL = FALSE;
BOOL AIBV_control_loop_finishing_AAAA_CONTROL_LOOP_IMPL = FALSE;
BOOL AIBV_position_error_AAAA_CONTROL_LOOP_IMPL = FALSE;
int AIBV_believed_position_AAAA_CONTROL_LOOP_IMPL = AIBV_CLOSED;
BOOL AIBV_control_loop_started_AAAA_CONTROL_LOOP_IMPL = FALSE;
int AIBV_expected_position_AAAA_CONTROL_LOOP_IMPL = AIBV_CLOSED;
BOOL AIBV_measuring_position_AAAA_CONTROL_LOOP_IMPL = FALSE;
int AIBV_required_position_AAAA_CONTROL_LOOP_IMPL = AIBV_CLOSED;
BOOL AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL = FALSE;
BOOL AIBV_have_done_a_temperature_control_loop_AAAA_CONTROL_LOOP_IMPL = FALSE;
int AIBV_temperature_LOCAL_AAAA_CONTROL_LOOP_IMPL = NOMINAL_TEMP;
BOOL AIBV_new_temperature_measurement_LOCAL_AAAA_CONTROL_LOOP_IMPL = FALSE;
BOOL DUMMY_VARIABLE_AAAA_CONTROL_LOOP_IMPL = FALSE;
const int priority_AAAA_CONTROL_LOOP_IMPL = 5;
int position_measurement = AIBV_CLOSED;


// Subroutines
void AAAA_CONTROL_LOOP_IMPL()
{
	while(TRUE)
	{
		// [Internal] Timer information for repeating or periodic tasks
		double internalPeriodicStartTime = omp_get_wtime();
		double internalTimeDifference;
		
		// Translated code
		AAAA_SHARED_IMPL_AIBV_get_temperature(&AIBV_temperature_LOCAL_AAAA_CONTROL_LOOP_IMPL, &AIBV_new_temperature_measurement_LOCAL_AAAA_CONTROL_LOOP_IMPL);


		if ((AIBV_state_AAAA_CONTROL_LOOP_IMPL == ARMED) && (AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_new_temperature_measurement_LOCAL_AAAA_CONTROL_LOOP_IMPL == TRUE) && ((FULLY_OPEN_TEMP <= AIBV_temperature_LOCAL_AAAA_CONTROL_LOOP_IMPL) && (AIBV_temperature_LOCAL_AAAA_CONTROL_LOOP_IMPL <= FULLY_CLOSED_TEMP)))
		{
			AIBV_required_position_AAAA_CONTROL_LOOP_IMPL = ((((AIBV_OPEN * FULLY_CLOSED_TEMP) - (AIBV_CLOSED * FULLY_OPEN_TEMP)) - (((AIBV_OPEN) - AIBV_CLOSED) * AIBV_temperature_LOCAL_AAAA_CONTROL_LOOP_IMPL)) / ((FULLY_CLOSED_TEMP) - FULLY_OPEN_TEMP));
			AIBV_new_temperature_measurement_LOCAL_AAAA_CONTROL_LOOP_IMPL = FALSE;
			AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL = TRUE;
		}
		else if ((AIBV_state_AAAA_CONTROL_LOOP_IMPL == ARMED) && (AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_new_temperature_measurement_LOCAL_AAAA_CONTROL_LOOP_IMPL == TRUE) && (AIBV_temperature_LOCAL_AAAA_CONTROL_LOOP_IMPL < FULLY_OPEN_TEMP))
		{
			AIBV_required_position_AAAA_CONTROL_LOOP_IMPL = AIBV_OPEN;
			AIBV_new_temperature_measurement_LOCAL_AAAA_CONTROL_LOOP_IMPL = FALSE;
			AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL = TRUE;
		}
		else if ((AIBV_state_AAAA_CONTROL_LOOP_IMPL == ARMED) && (AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_new_temperature_measurement_LOCAL_AAAA_CONTROL_LOOP_IMPL == TRUE) && (FULLY_CLOSED_TEMP < AIBV_temperature_LOCAL_AAAA_CONTROL_LOOP_IMPL))
		{
			AIBV_required_position_AAAA_CONTROL_LOOP_IMPL = AIBV_CLOSED;
			AIBV_new_temperature_measurement_LOCAL_AAAA_CONTROL_LOOP_IMPL = FALSE;
			AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL = TRUE;
		}
		else
		{
			DUMMY_VARIABLE_AAAA_CONTROL_LOOP_IMPL = FALSE;
		}


		AAAA_SHARED_IMPL_AIBV_set_new_temperature_measurement(AIBV_new_temperature_measurement_LOCAL_AAAA_CONTROL_LOOP_IMPL);


		if ((AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL == TRUE) && (AIBV_control_loop_started_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_position_error_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_believed_position_AAAA_CONTROL_LOOP_IMPL < AIBV_required_position_AAAA_CONTROL_LOOP_IMPL))
		{
			AIBV_action_AAAA_CONTROL_LOOP_IMPL = OPEN;
			AIBV_amount_AAAA_CONTROL_LOOP_IMPL = ((AIBV_required_position_AAAA_CONTROL_LOOP_IMPL) - AIBV_believed_position_AAAA_CONTROL_LOOP_IMPL);
			AIBV_expected_position_AAAA_CONTROL_LOOP_IMPL = AIBV_required_position_AAAA_CONTROL_LOOP_IMPL;
			AIBV_action_needs_to_measure_position_AAAA_CONTROL_LOOP_IMPL = TRUE;
			AIBV_control_loop_started_AAAA_CONTROL_LOOP_IMPL = TRUE;
		}
		else if ((AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL == TRUE) && (AIBV_control_loop_started_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_position_error_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_required_position_AAAA_CONTROL_LOOP_IMPL < AIBV_believed_position_AAAA_CONTROL_LOOP_IMPL))
		{
			AIBV_action_AAAA_CONTROL_LOOP_IMPL = CLOSE;
			AIBV_amount_AAAA_CONTROL_LOOP_IMPL = ((AIBV_believed_position_AAAA_CONTROL_LOOP_IMPL) - AIBV_required_position_AAAA_CONTROL_LOOP_IMPL);
			AIBV_expected_position_AAAA_CONTROL_LOOP_IMPL = AIBV_required_position_AAAA_CONTROL_LOOP_IMPL;
			AIBV_action_needs_to_measure_position_AAAA_CONTROL_LOOP_IMPL = TRUE;
			AIBV_control_loop_started_AAAA_CONTROL_LOOP_IMPL = TRUE;
		}
		else if ((AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL == TRUE) && (AIBV_control_loop_started_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_position_error_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_believed_position_AAAA_CONTROL_LOOP_IMPL == AIBV_required_position_AAAA_CONTROL_LOOP_IMPL))
		{
			AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL = FALSE;
		}
		else if ((AIBV_control_loop_started_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_position_error_AAAA_CONTROL_LOOP_IMPL == TRUE) && (AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL == TRUE))
		{
			AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL = FALSE;
		}
		else if ((AIBV_state_AAAA_CONTROL_LOOP_IMPL == DISARMING) && (AIBV_control_loop_started_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_position_error_AAAA_CONTROL_LOOP_IMPL == FALSE) && (AIBV_believed_position_AAAA_CONTROL_LOOP_IMPL != AIBV_CLOSED))
		{
			AIBV_action_AAAA_CONTROL_LOOP_IMPL = CLOSE;
			AIBV_amount_AAAA_CONTROL_LOOP_IMPL = ((AIBV_believed_position_AAAA_CONTROL_LOOP_IMPL) - AIBV_CLOSED);
			AIBV_expected_position_AAAA_CONTROL_LOOP_IMPL = AIBV_CLOSED;
			AIBV_action_needs_to_measure_position_AAAA_CONTROL_LOOP_IMPL = TRUE;
			AIBV_control_loop_started_AAAA_CONTROL_LOOP_IMPL = TRUE;
		}
		else
		{
			DUMMY_VARIABLE_AAAA_CONTROL_LOOP_IMPL = FALSE;
		}


		if ((AIBV_state_AAAA_CONTROL_LOOP_IMPL == ARMED) && (AIBV_action_AAAA_CONTROL_LOOP_IMPL == OPEN) && (AIBV_control_action_started_AAAA_CONTROL_LOOP_IMPL == FALSE) && (0 <= AIBV_amount_AAAA_CONTROL_LOOP_IMPL))
		{
			AIBV_control_action_started_AAAA_CONTROL_LOOP_IMPL = TRUE;
			AIBV_plant_IMPL_AIBV_open_start(AIBV_amount_AAAA_CONTROL_LOOP_IMPL);
		}
		else if ((AIBV_state_AAAA_CONTROL_LOOP_IMPL != DISARMED) && (AIBV_action_AAAA_CONTROL_LOOP_IMPL == CLOSE) && (AIBV_control_action_started_AAAA_CONTROL_LOOP_IMPL == FALSE) && (0 <= AIBV_amount_AAAA_CONTROL_LOOP_IMPL))
		{
			AIBV_control_action_started_AAAA_CONTROL_LOOP_IMPL = TRUE;
			AIBV_plant_IMPL_AIBV_close_start(AIBV_amount_AAAA_CONTROL_LOOP_IMPL);
		}
		else
		{
			DUMMY_VARIABLE_AAAA_CONTROL_LOOP_IMPL = FALSE;
		}


		if ((AIBV_state_AAAA_CONTROL_LOOP_IMPL == ARMED) && (AIBV_action_AAAA_CONTROL_LOOP_IMPL == OPEN) && (AIBV_control_action_started_AAAA_CONTROL_LOOP_IMPL == TRUE))
		{
			AIBV_action_AAAA_CONTROL_LOOP_IMPL = NOTHING;AIBV_amount_AAAA_CONTROL_LOOP_IMPL = 0;
			AIBV_control_action_started_AAAA_CONTROL_LOOP_IMPL = FALSE;
			AIBV_plant_IMPL_AIBV_open_end();
		}
		else if ((AIBV_state_AAAA_CONTROL_LOOP_IMPL != DISARMED) && (AIBV_action_AAAA_CONTROL_LOOP_IMPL == CLOSE) && (AIBV_control_action_started_AAAA_CONTROL_LOOP_IMPL == TRUE))
		{
			AIBV_action_AAAA_CONTROL_LOOP_IMPL = NOTHING;AIBV_amount_AAAA_CONTROL_LOOP_IMPL = 0;
			AIBV_control_action_started_AAAA_CONTROL_LOOP_IMPL = FALSE;
			AIBV_plant_IMPL_AIBV_close_end();
		}
		else
		{
			DUMMY_VARIABLE_AAAA_CONTROL_LOOP_IMPL = FALSE;
		}


		if ((AIBV_action_needs_to_measure_position_AAAA_CONTROL_LOOP_IMPL == TRUE) && (AIBV_action_AAAA_CONTROL_LOOP_IMPL == NOTHING) && (AIBV_measuring_position_AAAA_CONTROL_LOOP_IMPL == FALSE))
		{
			AIBV_measuring_position_AAAA_CONTROL_LOOP_IMPL = TRUE;
			AIBV_plant_IMPL_AIBV_measure_position_start();
		}
		else
		{
			DUMMY_VARIABLE_AAAA_CONTROL_LOOP_IMPL = FALSE;
		}


		if ((AIBV_action_needs_to_measure_position_AAAA_CONTROL_LOOP_IMPL == TRUE) && (AIBV_action_AAAA_CONTROL_LOOP_IMPL == NOTHING) && (AIBV_measuring_position_AAAA_CONTROL_LOOP_IMPL == TRUE))
		{
			AIBV_plant_IMPL_AIBV_measure_position_end(&position_measurement);
			if (((AIBV_CLOSED <= position_measurement) && (position_measurement <= AIBV_OPEN)) || (position_measurement == AIBV_UNKNOWN))
			{
				AIBV_action_needs_to_measure_position_AAAA_CONTROL_LOOP_IMPL = FALSE;
				AIBV_measured_position_AAAA_CONTROL_LOOP_IMPL = position_measurement;
				AIBV_position_measurement_to_check_AAAA_CONTROL_LOOP_IMPL = TRUE;
				AIBV_measuring_position_AAAA_CONTROL_LOOP_IMPL = FALSE;
			}
		}
		else
		{
			DUMMY_VARIABLE_AAAA_CONTROL_LOOP_IMPL = FALSE;
		}


		if ((AIBV_position_measurement_to_check_AAAA_CONTROL_LOOP_IMPL == TRUE) && (AIBV_measured_position_AAAA_CONTROL_LOOP_IMPL == AIBV_expected_position_AAAA_CONTROL_LOOP_IMPL))
		{
			AIBV_position_measurement_to_check_AAAA_CONTROL_LOOP_IMPL = FALSE;
			AIBV_position_error_AAAA_CONTROL_LOOP_IMPL = FALSE;AIBV_believed_position_AAAA_CONTROL_LOOP_IMPL = AIBV_measured_position_AAAA_CONTROL_LOOP_IMPL;
			AIBV_control_loop_finishing_AAAA_CONTROL_LOOP_IMPL = TRUE;
		}
		else if ((AIBV_position_measurement_to_check_AAAA_CONTROL_LOOP_IMPL == TRUE) && (AIBV_measured_position_AAAA_CONTROL_LOOP_IMPL != AIBV_expected_position_AAAA_CONTROL_LOOP_IMPL))
		{
			AIBV_position_measurement_to_check_AAAA_CONTROL_LOOP_IMPL = FALSE;
			AIBV_position_error_AAAA_CONTROL_LOOP_IMPL = TRUE;AIBV_believed_position_AAAA_CONTROL_LOOP_IMPL = AIBV_UNKNOWN;
			AIBV_control_loop_finishing_AAAA_CONTROL_LOOP_IMPL = TRUE;
			AIBV_plant_IMPL_AIBV_position_error_light_on();
		}
		else
		{
			DUMMY_VARIABLE_AAAA_CONTROL_LOOP_IMPL = FALSE;
		}


		if ((AIBV_control_loop_finishing_AAAA_CONTROL_LOOP_IMPL == TRUE))
		{
			AIBV_control_loop_started_AAAA_CONTROL_LOOP_IMPL = FALSE;
			AIBV_control_loop_finishing_AAAA_CONTROL_LOOP_IMPL = FALSE;
			AIBV_temperature_control_loop_started_AAAA_CONTROL_LOOP_IMPL = FALSE;
			AIBV_have_done_a_temperature_control_loop_AAAA_CONTROL_LOOP_IMPL = TRUE;
			AIBV_plant_IMPL_AIBV_finish_control_loop();
		}
		else
		{
			DUMMY_VARIABLE_AAAA_CONTROL_LOOP_IMPL = FALSE;
		}
		
		// [Internal] Code to monitor time between periodic tasks
		internalTimeDifference = omp_get_wtime() - internalPeriodicStartTime;
		__SLEEP(0 - (internalTimeDifference * 1000));
	}
}
