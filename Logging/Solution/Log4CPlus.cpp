#include "Log4CPlus.h"

/*
	To test the library, include "Log4CPlus.h" from an application project
	and call Log4CPlusTest().
	
	Do not forget to add the library to Project Dependencies in Visual Studio.
*/

static int s_Test = 0;

extern "C" int Log4CPlusTest();

int Log4CPlusTest()
{
	return ++s_Test;
}