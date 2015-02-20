
#include "ServoTask.h"

ServoTask::ServoTask(Logger logger) : TaskBase(logger)
{	
	servoDriver = PCA9685;
	successfulInit = false;
	i2cAddress = 0x40;
}

/// <summary>
/// Runs the task against
/// </summary>
/// <param name="args">List of int where
///		args[0] - Pin #
///		args[1] - Angle 0-180</param>
void ServoTask::Run(vector<int> args)
{
	if (args.size() < 2)
	{
		ostringstream msg;
		msg << "ServoTask::Run requires 2 arguments, only " << args.size() << " were used.";
		log.log(ERROR_LOG_LEVEL, msg.str());
		log.log(DEBUG_LOG_LEVEL,"Exiting ServoTask::Run");
		return;
	}
	if (args.size() > 2)
	{
		ostringstream msg;
		msg << "ServoTask::Run uses 2 arguments, " << args.size() << " were used, extras ignored.";
		log.log(WARN_LOG_LEVEL, msg.str());		
	}
	ostringstream msg;
	msg << "Entering ServoTask::Run(";
	for (int a = 0; a < args.size(); a++)
		msg << "args[" << a << "]==" << args[a] << " ";
	msg << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	
	int servoPin = args[0];
	int servoAngle = args[1];
	switch (servoDriver)
	{
		case PCA9685:
			pca9685Driver->SetAngle(servoPin, servoAngle);
			break;
	}
	msg << "Exiting ServoTask::Run(";
	for (int a = 0; a < args.size(); a++)
		msg << "args[" << a << "]==" << args[a] << " ";
	msg << ")";

}

void ServoTask::SetI2CAddress(int addr)
{
	i2cAddress = addr;
}

void ServoTask::Init()
{
	successfulInit = false;
	switch (servoDriver)
	{
		case PCA9685:
			pca9685Driver = new PCA9685Driver(log);
			if (!pca9685Driver->Init(i2cAddress))
			{
				successfulInit = false;
				break;
			}

	}
}

void ServoTask::SetServoDriver(EServoDriver driver)
{
	servoDriver = driver;
	if (pca9685Driver != NULL)
		free(pca9685Driver);
	Init();
}

ServoTask::~ServoTask()
{
}
