// MainEntry: MainEntry
#include "Common.h"
// Global defns
const int AIBV_UNKNOWN = -1;

const int AIBV_CLOSED = 0;

const int MAX_TEMP = 4;

const int MIN_TEMP = 0;

const int FULLY_CLOSED_TEMP = 2;

const int FULLY_OPEN_TEMP = 1;

const int NOMINAL_TEMP = 3;

const int AIBV_OPEN = 2;

int main (int argc, char *argv[])
{
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			AAAA_CONTROL_LOOP_IMPL();
		}
		#pragma omp section
		{
			AAAA_TEMPERATURE_IMPL();
		}
		#pragma omp section
		{
			AIBV_plant_IMPL();
		}
	}
}

