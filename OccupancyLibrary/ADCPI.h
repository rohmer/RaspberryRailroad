#pragma once
#include "wiringPi\wiringPiI2C.h"
#include "log4cplus\logger.h"
#include <map>
#include <sstream>
#include <stdlib.h>
#include <errno.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

#define ADC_CHANNEL1 0x9C
#define ADC_CHANNEL2 0xBC
#define ADC_CHANNEL3 0xDC
#define ADC_CHANNEL4 0xFC

enum ADCPiI2CAddress
{
	ADC68 = 0x68,
	ADC69 = 0x69,
	ADC6A = 0x6A,
	ADC6B = 0x6B,
	ADC6C = 0x6C,
	ADC6D = 0x6D,
	ADC6E = 0x6E,
	ADC6F = 0x6F

};

class ADCPI
{	
	private:
		std::map<int,int> adcAdapters;		// List of adapters and their file handles
											// Each adapter will have 4 channels
		std::vector<int> adcAdapterInOrder;

		Logger log;
		float varMultiplier;
		float varDivisor;
		
	public:
		ADCPI(Logger logger);	
		ADCPI() {};
		bool AddAdapter(ADCPiI2CAddress adapterAddress);
		float GetValue(int sensorNumber);
};