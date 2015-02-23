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
	LEDTask *ledTask = reinterpret_cast<LEDTask*>(tl.GetTask(TLEDTask));
	vector<int> args;
	args.push_back(1);
	args.push_back(1);
	args.push_back(1);
	args.push_back(1);

	ledTask->Draw(1, 1, true);
	ledTask->Run(args);

}
