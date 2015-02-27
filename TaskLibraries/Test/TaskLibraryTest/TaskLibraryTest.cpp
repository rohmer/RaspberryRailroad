#include <iostream>
#include "LEDTaskTests.h"
#include "log4cplus/logger.h"
#include "log4cplus/fileappender.h"
#include "LogHelper.h"

using namespace std;

int main(int argc, char *argv[])
{	
	Logger log = LogHelper::GetLogger(DEBUG_LOG_LEVEL, true, true, "TestLog.log", false, 0, "");
	LEDTaskTests ledTest(log);
	ledTest.BlinkRow1Col1();
	return 0;
}