#pragma once
#include "log4cplus\logger.h"
#include <map>
#include <limits>
#include <sstream>
#include <stdlib.h>
#include <errno.h>
#include "DetectorBase.h"
#include "ADCPI.h"

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

// Class for 
class OpticalDetector 
{	
	private:
		Logger log;
		ADCPI adcpi;
		struct detectorValues
		{
			float prevValues[32];
			float dMax;
			float dMin;
			float dRange;
			float dAvgValue;
			float dCurrentValue;
			float dThreshold;
			int dCurrentIndex;
			bool isActive;
			bool dFullArray;
		};

		vector<detectorValues> detectors;
		int detectorCount;

	public:
		OpticalDetector(Logger logger);
		bool AddDetectorArray(ADCPiI2CAddress ADCPiAddress);
		bool UpdateDetector(int detectorNumber);
		bool UpdateAllDetectors();
		bool DetectorActive(int detectorNumber);
		bool IsTriggered(int id);
};