#include "OpticalDetector.h"

OpticalDetector::OpticalDetector(Logger logger) :DetectorBase(log)
{
	log = logger;
	adcpi = ADCPI(log);
	detectorCount = 1;
}

bool OpticalDetector::AddDetectorArray(ADCPiI2CAddress ADCPiAddress)
{
	ostringstream msg;
	msg << "Entering OpticalDetector::AddDetectorArray(" << ADCPiAddress << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (!adcpi.AddAdapter(ADCPiAddress))
	{
		log.log(ERROR_LOG_LEVEL, "Error adding adapter");
		msg << "Entering OpticalDetector::AddDetectorArray(" << ADCPiAddress << "), rc==-1";
		return (false);
	};
	for (int dCount = 0; dCount < 4; dCount++)
	{
		detectorValues dv;
		dv.dAvgValue = 0;
		dv.dCurrentValue = 0;
		for (int a = 0; a < 32; a++)
			dv.prevValues[a] = 0;
		dv.dMax = std::numeric_limits<float>::min();
		dv.dMin = std::numeric_limits<float>::max();
		dv.dRange = 0;
		dv.dCurrentIndex = 0;
		dv.dThreshold = 0;
		dv.isActive = false;
		dv.dFullArray = false;
		detectors.push_back(dv);
		detectorCount++;
	}
	return (true);
}

bool OpticalDetector::UpdateDetector(int detectorNumber)
{
	ostringstream msg;
	msg << "Entering OpticalDetector::UpdateDetector(" << detectorNumber << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (detectorNumber > detectors.size())
	{
		msg.clear();
		msg << "detectorNumber(" << detectorNumber << ") > total detectors initialized: " << detectors.size();
		log.log(WARN_LOG_LEVEL, msg.str());
		msg.clear();
		msg << "Exiting OpticalDetector::UpdateDetector(" << detectorNumber << "), rc==false";
		log.log(DEBUG_LOG_LEVEL, msg.str());
		return false;
	}
	detectors[detectorNumber].dCurrentValue = adcpi.GetValue(detectorNumber);
	detectors[detectorNumber].prevValues[detectors[detectorNumber].dCurrentIndex] = detectors[detectorNumber].dCurrentValue;

	bool extremeChanged = false;
	float tempVal, newValue = 0;
	// Find lowest and highest values in array and store min and max
	for (int i = 0; i < 32; i++)
	{
		tempVal = detectors[detectorNumber].prevValues[i];
		if (tempVal < detectors[detectorNumber].dMin)
		{
			detectors[detectorNumber].dMin = tempVal;
			extremeChanged = true;
		}
		if (tempVal > detectors[detectorNumber].dMax)
		{
			detectors[detectorNumber].dMax = tempVal;
			extremeChanged = true;
		}
		newValue += tempVal;
	}

	// Update range
	detectors[detectorNumber].dRange = detectors[detectorNumber].dMax - detectors[detectorNumber].dMin;
	if (newValue > 0)
	{
		if (detectors[detectorNumber].dFullArray)
		{
			detectors[detectorNumber].dAvgValue = newValue / 32;
		}
		else
		{
			detectors[detectorNumber].dAvgValue = newValue / (detectors[detectorNumber].dCurrentIndex + 1);
		}

		// Adjust threshold (Average + (max-min*0.10)
		detectors[detectorNumber].dThreshold = detectors[detectorNumber].dAvgValue + (detectors[detectorNumber].dRange*0.35);

		// Adjust active flag
		detectors[detectorNumber].isActive = (detectors[detectorNumber].dCurrentValue > detectors[detectorNumber].dThreshold);
	}
	detectors[detectorNumber].dCurrentIndex++;
	if (detectors[detectorNumber].dCurrentIndex >= 32)
	{
		detectors[detectorNumber].dCurrentIndex = 0;
		detectors[detectorNumber].dFullArray = true;
	} 
	msg.clear();
	msg << "Exiting OpticalDetector::UpdateDetector(" << detectorNumber << "), rc==true";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return true;
}

bool OpticalDetector::UpdateAllDetectors()
{
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Entering OpticalDetector::UpdateAllDetectors()"));
	bool hadError = false;
	for (int a = 0; a < detectorCount; a++)
	{
		if (!UpdateDetector(a))
			hadError = true;
	}
	ostringstream msg;
	msg << "Exiting OpticalDetector::UpdateAllDetectors(), rc==" << hadError;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return hadError;
}

bool OpticalDetector::DetectorActive(int detectorNumber)
{
	return detectors[detectorNumber].isActive;
}

bool OpticalDetector::IsTriggered(int id)
{
	return detectors[id].isActive;
}