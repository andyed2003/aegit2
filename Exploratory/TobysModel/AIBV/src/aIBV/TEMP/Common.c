#include "Common.h"
// Platform specific function: sleep
// Supported platforms: Win32, UNIX
#ifdef __unix__
#include<unistd.h>
void __SLEEP(int period)
{
	if (period > 0)
	{
		usleep(period*1000);
	}
}
#elif defined _WIN32
#include<windows.h>
void __SLEEP(int period)
{
	if (period > 0)
	{
		Sleep(period);
	}
}
#endif

