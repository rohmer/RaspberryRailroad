#include "LEDTask.h"

LEDTask::LEDTask(Logger logger) :TaskBase(logger)
{	
	log.log(log4cplus::DEBUG_LOG_LEVEL, "LEDTask::LEDTask created");
	numDevice = 1;
	spiChannel = 0;
	spiSpeed = 1000000;
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
	int offset = (args[0]-1) * 64 + ((args[1]-1) * 8) + args[2]-1;
	if (args[2] == 0)
		maxData[offset] = 0;
	else
		maxData[offset] = 1;
}

void LEDTask::sendData()
{
	TxBufferIndex = 0;
	for (int dNum = 0; dNum < numDevice; dNum++)
	{
		for (int row = 0; row < 8; row++)
		{
			uint8_t total=0;
			for (int col = 0; col < 8; col++)
			{
				if (maxData[dNum * 64 + row * 8 + col] == 1)
					total += col ^ 2;
			}
			TxBuffer[TxBufferIndex] = (char)total;
		}
	}
	int rc=wiringPiSPIDataRW(spiFD, TxBuffer, TxBufferIndex);
	TxBufferIndex = 0;
	ostringstream msg;
	msg << "Wrote: " << rc << " to SPI bus";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void LEDTask::SetSPIChannel(int channel)
{
	spiChannel = channel;
}

void LEDTask::SetSPISpeed(int speed)
{
	spiSpeed = speed;
}

void LEDTask::Init()
{		
	// Create a clear instance
	for (int a = 0; a < (numDevice * 16); a++)
	{
		maxData.push_back(0);		
	}
	TxBuffer = new unsigned char[1024];	
	if ((spiFD = wiringPiSPISetup(spiChannel, spiSpeed)) < 0)
	{
		ostringstream msg;
		msg << "Error opening SPI port, rc==" << errno;
		log.log(ERROR_LOG_LEVEL, msg.str());
		successfulInit = false;
		return;
	}
}


void LEDTask::Clear()
{
}

void LEDTask::SetNumDevice(int numberOfMax)
{
	numDevice = numberOfMax;
	log.log(DEBUG_LOG_LEVEL, "numDevice set to: " + numDevice);
	maxData.clear();
	for (int a = 0; a < (numDevice * 16); a++)
	{
		maxData.push_back(0);
	}
	sendData();
}

void LEDTask::Draw(int device, int column, int row, bool powerMode)
{
	int offset = device * 64 + ((row-1) * 8) + column-1;
	if (powerMode)
		maxData[offset] = 1;
	else
		maxData[offset] = 0;
	sendData();
}

LEDTask::~LEDTask()
{
	log.log(DEBUG_LOG_LEVEL, "Running ~LEDTask()");
	free(TxBuffer);	
	log.log(DEBUG_LOG_LEVEL, "~LEDTask() complete");
}