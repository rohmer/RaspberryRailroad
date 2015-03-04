#include "LEDTask.h"

LEDTask::LEDTask(Logger logger) :TaskBase(logger)
{	
	log.log(log4cplus::DEBUG_LOG_LEVEL, "LEDTask::LEDTask created");	
	_spiChannel = 0;
	_spiSpeed = 1000000;
	_maxCount = 1;
	maxClass = new max7219(log, _maxCount);
}

/// <summary>
/// Runs the specified arguments.
/// </summary>
/// <param name="args">Args [0] - Max Device (First device is 1)
///					   Args [1] - Row (First row is 1)
///					   Args [2] - Col (First col is 1)
///					   Args [3] - 0 off, 1 on
/// </param>
void LEDTask::Run(vector<int> args)
{
	ostringstream msg;
	ostringstream argstring;
	for (int a = 0; a < args.size() - 1; a++)
		argstring << args[a] << ",";
	argstring << args[args.size() - 1];
	msg << "Entering LEDTask::Run(" << argstring << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());	
	if (args.size() < 4)
	{
		msg.clear();
		msg << "LEDTask::Run() expects 4 arguments, received: " << args.size();
		log.log(ERROR_LOG_LEVEL, msg.str());
		msg.clear();
		msg << "Exiting LEDTask::Run(" << argstring << ")";
		log.log(DEBUG_LOG_LEVEL, msg.str());
		return;
	}
	int row = args[1];
	int col = args[2];
	bool power = true;
	if (args[3] == 0)
		power = false;
	Draw(col, row, power);
	msg.clear();
	msg << "Exiting LEDTask::Run(" << argstring << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());	
}

void LEDTask::SetMaxParam(int spiSpeed, int spiChannel, int maxCount)
{
	_spiSpeed = spiSpeed;
	_spiChannel = spiChannel;
	_maxCount = maxCount;
	maxClass->SetNumDevices(_maxCount);
	maxClass->SetSPIChannel(_spiChannel);
	maxClass->SetSPISpeed(_spiSpeed);
}

void LEDTask::Clear()
{
	maxClass->Clear();
}

void LEDTask::Init()
{				
	Clear();							// Turn everything off
}

void LEDTask::Draw(int column, int row, bool powerMode)
{	
	ostringstream msg;
	ostringstream args;

}

LEDTask::~LEDTask()
{
	log.log(DEBUG_LOG_LEVEL, "Running ~LEDTask()");	
	log.log(DEBUG_LOG_LEVEL, "~LEDTask() complete");
}