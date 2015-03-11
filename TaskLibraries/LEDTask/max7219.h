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
#include "max7219Container.h"

using namespace std;
using namespace log4cplus;

class max7219
{
	const static char NoOp = 0x00;
	const static char Digit0 = 0x01;
	const static char Digit1 = 0x02;
	const static char Digit2 = 0x03;
	const static char Digit3 = 0x04;
	const static char Digit4 = 0x05;
	const static char Digit5 = 0x06;
	const static char Digit6 = 0x07;
	const static char Digit7 = 0x08;
	const static char DecodeMode = 0x09;
	const static char Intensity = 0x0A;
	const static char ScanLimit = 0x0B;
	const static char ShutDown = 0x0C;
	const static char DisplayTest = 0x0F;
	

	private:
		Logger log;
		int numOfDevices;
		int spiChannel;
		int spiSpeed;
		int spiFD;
		bool initialized;
		unsigned char spiData[16];
		unsigned char status[64];

		void initialize();
		void spiTransfer(int addr, volatile unsigned char opcode, volatile unsigned char data);

	public:
		max7219(Logger logger, int numberOfDevices);
		void SetNumDevices(int value); 
		void SetSPIChannel(int value); 
		void SetSPISpeed(int value);
		void Shutdown(int maxDevice, bool value);
		void SetIntensity(int maxDevice, int value);

		void Draw(int row, int col, int maxDevice, bool powerMode);
		void Draw(int row, int col, bool powerMode);					// Draw on all devices
		void Clear(int maxDevice);
		void Clear();
		~max7219();


};