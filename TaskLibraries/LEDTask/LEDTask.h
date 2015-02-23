#pragma once
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include "../TaskBase/TaskBase.h"
#include <wiringPi.h>
#include <signal.h>
#include <wiringPiSPI.h>
#include "max7219.h"
#include <stdlib.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

// The Max7219 Registers :

#define DECODE_MODE   0x09                       
#define INTENSITY     0x0a                        
#define SCAN_LIMIT    0x0b                        
#define SHUTDOWN      0x0c                        
#define DISPLAY_TEST  0x0f              

// Used in conjunction with a Max7219 to control multiple LEDs
class LEDTask : public TaskBase
{
	private:
		bool successfulInit;
		int _data, _clock, _load;
		int numDevice;
		int spiFD;
		void max7219Send(unsigned char reg_number, unsigned char dataout);
		void send16bits(unsigned short output);

	public:
		LEDTask(Logger logger);
		void Run(vector<int> args);	
		void Init();
		void SetPins(int data, int clock, int load);
		void Draw(int column, int row, bool powerMode);
		~LEDTask();
};