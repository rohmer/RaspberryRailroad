#pragma once
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include "../TaskBase/TaskBase.h"
#include <wiringPi.h>
#include <signal.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

// Used in conjunction with a Max7219 to control multiple LEDs
class LEDTask : TaskBase
{
	private:
		bool successfulInit;
		int _ce1, _ce0, _sclk, _miso, _mosi, _rxd, _txd, _scl, _sda;
		int rows[8];
		int columns[8];

	public:
		LEDTask(Logger logger);
		void Run();	
		void Init();
		void SetPins(int ce1, int ce0, int sclk, int miso, int mosi, int rxd, int txd, int scl, int sda);
		void Clear();
		void Draw(int column, int row, bool powerMode);
		~LEDTask();
};