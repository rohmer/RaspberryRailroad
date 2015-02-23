#include "LEDTask.h"

LEDTask::LEDTask(Logger logger) :TaskBase(logger)
{	
	log.log(log4cplus::DEBUG_LOG_LEVEL, "LEDTask::LEDTask created");
	numDevice = 1;
	SetPins(0, 3, 4);
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
	if (!successfulInit)
		Init();
}

void LEDTask::SetPins(int data, int clock, int load)
{
	_data = data;
	_clock = clock;
	_load = load;
}

void LEDTask::max7219Send(unsigned char reg_number, unsigned char dataout)
{
	digitalWrite(_load, 1);  // set LOAD 1 to start
	send16bits((reg_number << 8) + dataout);   // send 16 bits ( reg number + dataout )
	digitalWrite(_load, 0);  // LOAD 0 to latch
	digitalWrite(_load, 1);  // set LOAD 1 to finish
}

void LEDTask::send16bits(unsigned short output)
{
	unsigned char i;
	digitalWrite(_load, LOW);
	for (i = 16; i > 0; i--)
	{
		unsigned short mask = 1 << (i - 1); // calculate bitmask

		digitalWrite(_clock, 0);  // set clock to 0
								 // Send one bit on the data pin
		if (output & mask)
			digitalWrite(_data, 1);
		else
			digitalWrite(_data, 0);
		digitalWrite(_clock, 1);  // set clock to 1
	}
}


void LEDTask::Init()
{			
	pinMode(_data, OUTPUT);
	pinMode(_clock, OUTPUT);
	pinMode(_load, OUTPUT);
	max7219Send(SCAN_LIMIT, 7);
	max7219Send(DISPLAY_TEST, 0);
	max7219Send(INTENSITY, 15);
	max7219Send(SHUTDOWN, 1);
}

void LEDTask::Draw(int column, int row, bool powerMode)
{	
	max7219Send(row, column);
}

LEDTask::~LEDTask()
{
	log.log(DEBUG_LOG_LEVEL, "Running ~LEDTask()");
	max7219Send(SHUTDOWN, 0);
	log.log(DEBUG_LOG_LEVEL, "~LEDTask() complete");
}