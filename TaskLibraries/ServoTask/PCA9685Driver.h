#pragma once
#include <log4cplus\logger.h>
#include "wiringPiI2C.h"
#include <sstream>
#include <iostream>
#include <cerrno>
#include "pca9685.h"
#include "wiringPi.h"

using namespace log4cplus;
using namespace log4cplus::helpers;

#define MAX_PWM 4096

class PCA9685Driver 
{
	private:
		int controllerFD;
		int frequency;
		int pinBase;
		int calcTicks(float inputMs);
		Logger log;

	public:
		PCA9685Driver(Logger logger);
		~PCA9685Driver();
		bool Init(int address);		
		void SetFrequency(int freq);
		void SetPinBase(int pBase);
		void SetAngle(int pin, int angle);
};