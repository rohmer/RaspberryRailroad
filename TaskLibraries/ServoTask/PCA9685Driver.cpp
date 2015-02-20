#include "PCA9685Driver.h"

PCA9685Driver::PCA9685Driver(Logger logger) 
{
	frequency = 50;	
	pinBase = 300;
	log = logger;
}

PCA9685Driver::~PCA9685Driver()
{	
}

void PCA9685Driver::SetFrequency(int freq)
{
	frequency = freq;
}

void PCA9685Driver::SetPinBase(int pBase)
{
	pinBase = pBase;
}

int PCA9685Driver::calcTicks(float impulseMS)
{
	float cycleMs = 1000.0f / frequency;
	return(int)(MAX_PWM * impulseMS / cycleMs + 0.5f);
}

// Sets up communications
// You can use i2cdetect to find out the I2C number of the device
bool PCA9685Driver::Init(int address)
{
	log.log(DEBUG_LOG_LEVEL, "Initializing PCA9685Driver");
	controllerFD = pca9685Setup(pinBase, address, frequency);

	if (controllerFD == -1)
	{
		std::ostringstream msg;
		msg << "Error initalizing PCA9685Driver, will be unavailable.  Errno==" << errno;
		log.log(ERROR_LOG_LEVEL, msg.str());
		
		return false;
	}
	
	pca9685PWMReset(controllerFD);

	std::ostringstream msg;
	msg << "PCA9685Driver initialized, fd==" << controllerFD;
	
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return true;
}

void PCA9685Driver::SetAngle(int pin, int angle)
{
	int value = 50;
	if (angle != 0)
	{
		int value = 50 + (float)(50 * ((float)angle / 180));
	}
	pwmWrite(pin, value);
}