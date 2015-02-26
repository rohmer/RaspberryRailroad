#pragma once

#include "TaskLibrary.h"
#include <log4cplus/logger.h>
#include <unistd.h>

class LEDTaskTests
{
	private:
		Logger log;

	public:
		LEDTaskTests(Logger logger);
		void BlinkRow1Col1();
		

};