#pragma once
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include "../TaskBase/TaskBase.h"

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

class LEDTask : TaskBase
{
	private:

	public:
		LEDTask(Logger logger) : TaskBase(logger);
		void Run();

};