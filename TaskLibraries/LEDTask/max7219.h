#pragma once
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <log4cplus/logger.h>
#include "../TaskBase/TaskBase.h"
#include <signal.h>
#include "max7219.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <map>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

using namespace std;
using namespace log4cplus;

class max7219
{
	#define INTENSITY     0x0a
	#define SHUTDOWN      0x0c   
	
	private:
		Logger log;		
		int spiChannel;
		int spiSpeed;
		int spiFD;
		bool initialized;
		unsigned char spiData[16];
		bool status[64];
		char *device;
		uint8_t mode;
		uint8_t bits;

		void initialize();
		void spiTransfer(unsigned char opcode, unsigned char data);
		
	public:
		max7219(Logger logger);
		void SetSPIChannel(int value); 
		void SetSPISpeed(int value);
		void Shutdown(bool value);
		void SetIntensity(int value);
		void SetSPIBits(uint8_t spiBits) { bits = spiBits; }

		void Draw(int row, int col, bool powerMode);		
		void Clear();
		~max7219();


};