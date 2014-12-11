#include "Common.h"
// Task: CONTROL_LOOP_IMPL

// Variables and constants
AIBV_STATES AIBV_state_CONTROL_LOOP_IMPL = DISARMED;
AIBV_ACTIONS AIBV_action_CONTROL_LOOP_IMPL = NOTHING;
int AIBV_amount_CONTROL_LOOP_IMPL = 0;
BOOL AIBV_control_action_started_CONTROL_LOOP_IMPL = FALSE;
BOOL AIBV_action_needs_to_measure_position_CONTROL_LOOP_IMPL = FALSE;
int AIBV_measured_position_CONTROL_LOOP_IMPL = AIBV_UNKNOWN;
BOOL AIBV_position_measurement_to_check_CONTROL_LOOP_IMPL = FALSE;
BOOL AIBV_control_loop_finishing_CONTROL_LOOP_IMPL = FALSE;
BOOL AIBV_position_error_CONTROL_LOOP_IMPL = FALSE;
int AIBV_believed_position_CONTROL_LOOP_IMPL = AIBV_CLOSED;
BOOL AIBV_control_loop_started_CONTROL_LOOP_IMPL = FALSE;
int AIBV_expected_position_CONTROL_LOOP_IMPL = AIBV_CLOSED;
BOOL AIBV_measuring_position_CONTROL_LOOP_IMPL = FALSE;
int AIBV_required_position_CONTROL_LOOP_IMPL = AIBV_CLOSED;
BOOL AIBV_temperature_control_loop_started_CONTROL_LOOP_IMPL = FALSE;
BOOL AIBV_have_done_a_temperature_control_loop_CONTROL_LOOP_IMPL = FALSE;
int AIBV_required_positionInitialValue_CONTROL_LOOP_IMPL;
const int priority_CONTROL_LOOP_IMPL = 5;


// Subroutines
void CONTROL_LOOP_IMPL()
{
	while(TRUE)
	{
		// [Internal] Timer information for repeating or periodic tasks
		double internalPeriodicStartTime = omp_get_wtime();
		double internalTimeDifference;
		
		// Translated code
		AIBV_required_positionInitialValue_CONTROL_LOOP_IMPL = AIBV_required_position_CONTROL_LOOP_IMPL;
		AIBV_required_position_CONTROL_LOOP_IMPL = ((((AIBV_OPEN * FULLY_CLOSED_TEMP) - (AIBV_CLOSED * FULLY_OPEN_TEMP)) - (((AIBV_OPEN) - AIBV_CLOSED) * temperature)) / ((FULLY_CLOSED_TEMP) - FULLY_OPEN_TEMP));
		AIBV_temperature_control_loop_started_CONTROL_LOOP_IMPL = TRUE;
		if ((AIBV_state_CONTROL_LOOP_IMPL == ARMED) && (AIBV_temperature_control_loop_started_CONTROL_LOOP_IMPL == FALSE) && ((FULLY_OPEN_TEMP <= temperature) && (temperature <= FULLY_CLOSED_TEMP)))
		{
			SHARED_IMPL_AIBV_calculate_position_partial(&AIBV_required_positionInitialValue_CONTROL_LOOP_IMPL);
		}
		AIBV_required_position_CONTROL_LOOP_IMPL = AIBV_OPEN;
		AIBV_temperature_control_loop_started_CONTROL_LOOP_IMPL = TRUE;
		if ((AIBV_state_CONTROL_LOOP_IMPL == ARMED) && (AIBV_temperature_control_loop_started_CONTROL_LOOP_IMPL == FALSE) && (temperature < FULLY_OPEN_TEMP))
		{
			SHARED_IMPL_AIBV_calculate_position_open();
		}
		AIBV_required_position_CONTROL_LOOP_IMPL = AIBV_CLOSED;
		AIBV_temperature_control_loop_started_CONTROL_LOOP_IMPL = TRUE;
		if ((AIBV_state_CONTROL_LOOP_IMPL == ARMED) && (AIBV_temperature_control_loop_started_CONTROL_LOOP_IMPL == FALSE) && (FULLY_CLOSED_TEMP < temperature))
		{
			SHARED_IMPL_AIBV_calculate_position_closed();
		}
		if ((AIBV_temperature_control_loop_started_CONTROL_LOOP_IMPL == TRUE) && (AIBV_control_loop_started_CONTROL_LOOP_IMPL == FALSE) && (AIBV_position_error_CONTROL_LOOP_IMPL == FALSE) && (AIBV_believed_position_CONTROL_LOOP_IMPL < AIBV_required_position_CONTROL_LOOP_IMPL))
		{
			AIBV_action_CONTROL_LOOP_IMPL = OPEN;
			AIBV_amount_CONTROL_LOOP_IMPL = ((AIBV_required_position_CONTROL_LOOP_IMPL) - AIBV_believed_position_CONTROL_LOOP_IMPL);
			AIBV_expected_position_CONTROL_LOOP_IMPL = AIBV_required_position_CONTROL_LOOP_IMPL;
			AIBV_action_needs_to_measure_position_CONTROL_LOOP_IMPL = TRUE;
			AIBV_control_loop_started_CONTROL_LOOP_IMPL = TRUE;
		}
		else if ((AIBV_temperature_control_loop_started_CONTROL_LOOP_IMPL == TRUE) && (AIBV_control_loop_started_CONTROL_LOOP_IMPL == FALSE) && (AIBV_position_error_CONTROL_LOOP_IMPL == FALSE) && (AIBV_required_position_CONTROL_LOOP_IMPL < AIBV_believed_position_CONTROL_LOOP_IMPL))
		{
			AIBV_action_CONTROL_LOOP_IMPL = CLOSE;
			AIBV_amount_CONTROL_LOOP_IMPL = ((AIBV_believed_position_CONTROL_LOOP_IMPL) - AIBV_required_position_CONTROL_LOOP_IMPL);
			AIBV_expected_position_CONTROL_LOOP_IMPL = AIBV_required_position_CONTROL_LOOP_IMPL;
			AIBV_action_needs_to_measure_position_CONTROL_LOOP_IMPL = TRUE;
			AIBV_control_loop_started_CONTROL_LOOP_IMPL = TRUE;
		}
		else
		{
			AIBV_action_CONTROL_LOOP_IMPL = CLOSE;
			AIBV_amount_CONTROL_LOOP_IMPL = ((AIBV_believed_position_CONTROL_LOOP_IMPL) - AIBV_CLOSED);
			AIBV_expected_position_CONTROL_LOOP_IMPL = AIBV_CLOSED;
			AIBV_action_needs_to_measure_position_CONTROL_LOOP_IMPL = TRUE;
			AIBV_control_loop_started_CONTROL_LOOP_IMPL = TRUE;
		}
		if ((AIBV_state_CONTROL_LOOP_IMPL == ARMED) && (AIBV_action_CONTROL_LOOP_IMPL == OPEN) && (AIBV_control_action_started_CONTROL_LOOP_IMPL == FALSE) && (0 <= amount))
		{
			AIBV_control_action_started_CONTROL_LOOP_IMPL = TRUE;
		}
		else
		{
			AIBV_control_action_started_CONTROL_LOOP_IMPL = TRUE;
		}
		if ((AIBV_state_CONTROL_LOOP_IMPL == ARMED) && (AIBV_action_CONTROL_LOOP_IMPL == OPEN) && (AIBV_control_action_started_CONTROL_LOOP_IMPL == TRUE))
		{
			AIBV_action_CONTROL_LOOP_IMPL = NOTHING;AIBV_amount_CONTROL_LOOP_IMPL = 0;
			AIBV_control_action_started_CONTROL_LOOP_IMPL = FALSE;
		}
		else
		{
			AIBV_action_CONTROL_LOOP_IMPL = NOTHING;AIBV_amount_CONTROL_LOOP_IMPL = 0;
			AIBV_control_action_started_CONTROL_LOOP_IMPL = FALSE;
		}
		AIBV_measuring_position_CONTROL_LOOP_IMPL = TRUE;
		AIBV_action_needs_to_measure_position_CONTROL_LOOP_IMPL = FALSE;
		AIBV_measured_position_CONTROL_LOOP_IMPL = position_measurement;
		AIBV_position_measurement_to_check_CONTROL_LOOP_IMPL = TRUE;
		AIBV_measuring_position_CONTROL_LOOP_IMPL = FALSE;
		if ((AIBV_position_measurement_to_check_CONTROL_LOOP_IMPL == TRUE) && (AIBV_measured_position_CONTROL_LOOP_IMPL == AIBV_expected_position_CONTROL_LOOP_IMPL))
		{
			AIBV_position_measurement_to_check_CONTROL_LOOP_IMPL = FALSE;
			AIBV_position_error_CONTROL_LOOP_IMPL = FALSE;AIBV_believed_position_CONTROL_LOOP_IMPL = AIBV_measured_position_CONTROL_LOOP_IMPL;
			AIBV_control_loop_finishing_CONTROL_LOOP_IMPL = TRUE;
		}
		else
		{
			AIBV_position_measurement_to_check_CONTROL_LOOP_IMPL = FALSE;
			AIBV_position_error_CONTROL_LOOP_IMPL = TRUE;AIBV_believed_position_CONTROL_LOOP_IMPL = AIBV_UNKNOWN;
			AIBV_control_loop_finishing_CONTROL_LOOP_IMPL = TRUE;
		}
		AIBV_control_loop_started_CONTROL_LOOP_IMPL = FALSE;
		AIBV_control_loop_finishing_CONTROL_LOOP_IMPL = FALSE;
		AIBV_temperature_control_loop_started_CONTROL_LOOP_IMPL = FALSE;
		AIBV_have_done_a_temperature_control_loop_CONTROL_LOOP_IMPL = TRUE;
		
		// [Internal] Code to monitor time between periodic tasks
		internalTimeDifference = omp_get_wtime() - internalPeriodicStartTime;
		__SLEEP(0 - (internalTimeDifference * 1000));
	}
}
