#pragma once
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include "../TaskBase/TaskBase.h"
#include <wiringPi.h>
#include <signal.h>
#include <wiringPiSPI.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

// Used in conjunction with a Max7219 to control multiple LEDs
class ServoTask : TaskBase
{
private:
	bool successfulInit;
	int _ce1, _ce0, _sclk, _miso, _mosi, _rxd, _txd, _scl, _sda;
	int rows[8];
	int columns[8];
	int numDevice;

public:
	ServoTask(Logger logger);
	void Run(int args[]);
	void Init();		
	void Clear();	
	~ServoTask();
};