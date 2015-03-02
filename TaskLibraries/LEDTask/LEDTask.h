#pragma once
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include "../TaskBase/TaskBase.h"
#include "../../wiringPi/wiringPi/wiringPi.h"
#include <signal.h>
#include "../../wiringPi/wiringPi/wiringPiSPI.h"
#include "max7219.h"
#include <stdlib.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

// Used in conjunction with a Max7219 to control multiple LEDs
class LEDTask : public TaskBase
{
	private:
		bool successfulInit;
		int _spiSpeed, _spiChannel, _maxCount;
		int numDevice;
		int spiFD;
		void internalDraw();
		bool rows[8];
		bool cols[8];

	public:
		LEDTask(Logger logger);
		void Run(vector<int> args);	
		void SetMaxParam(int sipSpeed, int spiChannel, int maxCount);
		void Init();
		void Draw(int column, int row, bool powerMode);
		void Clear();
		std::string GetTaskName() { return "LEDTask";  }
		~LEDTask();
};