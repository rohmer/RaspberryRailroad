#include "LEDTaskTests.h"

LEDTaskTests::LEDTaskTests(Logger logger)
{
	log = logger;
	string msg = "LEDTaskTests initialized";
	log.log(DEBUG_LOG_LEVEL,msg);
}

void LEDTaskTests::BlinkRow1Col1()
{
	TaskLibrary tl(log);
	//LEDTask * ledTask = tl.GetTask(TLEDTask);
}
