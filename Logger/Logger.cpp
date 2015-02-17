#include "Logger.h"

/*
	To test the library, include "Logger.h" from an application project
	and call LoggerTest().
	
	Do not forget to add the library to Project Dependencies in Visual Studio.
*/

static int s_Test = 0;

extern "C" int LoggerTest();

int LoggerTest()
{
	return ++s_Test;
}