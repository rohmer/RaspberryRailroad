#pragma once
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include "../TaskBase/TaskBase.h"
#include <wiringPi.h>
#include <signal.h>
#include <wiringPiSPI.h>
#include "PCA9685Driver.h"

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

// Initally only going to support the one I own :-)

// Notes:
//   * Requires libi2cdev
//	 * Also requires i2c drivers loaded into kernel
//	 Process to get working:
//		•$ sudo apt - get install i2c - tools
//		•$ sudo apt - get install libi2c - dev
//		•$ sudo nano /etc/modprobe.d/raspi-blacklist.conf
//			comment the line of
//			#blacklist i2c - bcm2708
//		•$ sudo nano /etc/modules
//			add the lines :
//			i2c-bcm2708
//			i2c-dev
//		•reboot

enum EServoDriver { PCA9685 };

class ServoDriverBase;

class ServoTask : TaskBase
{
private:
	bool successfulInit;
	EServoDriver servoDriver;	
	PCA9685Driver* pca9685Driver;
	int i2cAddress;

public:
	ServoTask(Logger logger);
	void Run(vector<int> args);
	void SetI2CAddress(int addr);
	void Init();		
	void SetServoDriver(EServoDriver driver);
	~ServoTask();
};