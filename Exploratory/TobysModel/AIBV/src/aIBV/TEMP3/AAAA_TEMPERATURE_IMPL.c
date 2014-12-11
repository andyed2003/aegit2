#include "Common.h"
// Task: AAAA_TEMPERATURE_IMPL

// Variables and constants
BOOL AIBV_temperature_needs_checking_AAAA_TEMPERATURE_IMPL = FALSE;
BOOL AIBV_temperature_error_AAAA_TEMPERATURE_IMPL = FALSE;
int AIBV_buffered_temperature_AAAA_TEMPERATURE_IMPL = NOMINAL_TEMP;
const int priority_AAAA_TEMPERATURE_IMPL = 5;
int temperature = NOMINAL_TEMP;


// Subroutines
void AAAA_TEMPERATURE_IMPL()
{
	while(TRUE)
	{
		// [Internal] Timer information for repeating or periodic tasks
		double internalPeriodicStartTime = omp_get_wtime();
		double internalTimeDifference;
		
		// Translated code
		AIBV_plant_IMPL_AIBV_temperature_measurement(&temperature);
		AIBV_buffered_temperature_AAAA_TEMPERATURE_IMPL = temperature;
		AIBV_temperature_needs_checking_AAAA_TEMPERATURE_IMPL = TRUE;


		if ((AIBV_temperature_needs_checking_AAAA_TEMPERATURE_IMPL == TRUE) && ((MIN_TEMP <= AIBV_buffered_temperature_AAAA_TEMPERATURE_IMPL) && (AIBV_buffered_temperature_AAAA_TEMPERATURE_IMPL <= MAX_TEMP)))
		{
			AIBV_temperature_error_AAAA_TEMPERATURE_IMPL = FALSE;
			AIBV_temperature_needs_checking_AAAA_TEMPERATURE_IMPL = FALSE;
			AAAA_SHARED_IMPL_AIBV_check_temperature_ok(AIBV_buffered_temperature_AAAA_TEMPERATURE_IMPL);
		}
		else
		{
			AIBV_temperature_error_AAAA_TEMPERATURE_IMPL = TRUE;
			AIBV_temperature_needs_checking_AAAA_TEMPERATURE_IMPL = FALSE;
		}
		
		// [Internal] Code to monitor time between periodic tasks
		internalTimeDifference = omp_get_wtime() - internalPeriodicStartTime;
		__SLEEP(0 - (internalTimeDifference * 1000));
	}
}
