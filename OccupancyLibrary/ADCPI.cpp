#include "ADCPI.h"

ADCPI::ADCPI(Logger logger)
{
	log = logger;
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("ADCPI initalized, dont forget to add adapters"));
	
	// setup multiplier based on input voltage and divisor value
	// From:
	// https://github.com/abelectronicsuk/ABElectronics_C_Libraries/blob/master/adcpiv2.c
	varMultiplier = (2.4705882 / varDivisor) / 1000;
}

bool ADCPI::AddAdapter(ADCPiI2CAddress adapterAddress)
{
	ostringstream msg;
	msg << "Entering ADCPI::AddAdapter(" << adapterAddress << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());

	for (int a = 0; a < adcAdapters.size(); a++)
	{
		if (adcAdapters[a] == adapterAddress)
		{
			msg.clear();
			msg << "Adapter: " << adapterAddress << " already added, timidly not adding it again.";
			log.log(WARN_LOG_LEVEL, msg.str());
			return false;
		}
	}
	int rc = wiringPiI2CSetup(adapterAddress);
	if (rc == -1)
	{
		msg.clear();
		msg << "Error initalizing adapter: " << adapterAddress << ", errno: " << errno;
		log.log(ERROR_LOG_LEVEL, msg.str());
		return false;
	}
	adcAdapters.insert(pair<int, int>(adapterAddress, rc));
	adcAdapterInOrder.push_back(adapterAddress);
	msg.clear();
	msg << "Entering ADCPI::AddAdapter(" << adapterAddress << "), rc==true";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return true;
}

float ADCPI::GetValue(int sensorNumber)
{
	ostringstream msg;
	msg << "Entering ADCPI::GetValue(" << sensorNumber << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (sensorNumber + 1 > (adcAdapters.size() * 4))
	{
		msg.clear();
		msg << "Sensor: " << sensorNumber << " requested and only: " << (adcAdapters.size() * 4) << " available";
		log.log(WARN_LOG_LEVEL, msg.str());
		msg.clear();
		msg << "Exiting ADCPI::GetValue(" << sensorNumber << "), rc==-1";
		log.log(DEBUG_LOG_LEVEL, msg.str());
		return (-1);
	}

	div_t d = div(sensorNumber, 4);
	int adapterNumber = d.quot;
	int channelNum = d.rem;

	int channelVal;
	switch (channelNum)
	{
		case(0) :
			channelVal = ADC_CHANNEL1;
			break;
		case(1) :
			channelVal = ADC_CHANNEL2;
			break;
		case(2) :
			channelVal = ADC_CHANNEL3;
			break;
		case(3) :
			channelVal = ADC_CHANNEL4;
			break;
	}
	int value = wiringPiI2CReadReg16(adcAdapterInOrder[adapterNumber], channelVal);
	
	msg.clear();
	
	msg << "Exiting ADCPI::GetValue(" << sensorNumber << "), rc==" <<(value*varMultiplier);
	log.log(DEBUG_LOG_LEVEL, msg.str());
		
	return (value*varMultiplier);
}