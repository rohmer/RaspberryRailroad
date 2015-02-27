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

void LEDTask::Clear()
{
	for (int a = 0; a < 8; a++)
	{
		rows[a] = false;
		cols[a] = false;
	}
	internalDraw();
}

void LEDTask::putByte(unsigned char data)
{
	unsigned char i = 8;
	unsigned char mask;
	while (i > 0)
	{
		mask = 0x01 << (i - 1);				// Get bitmask
		digitalWrite(_clock, LOW);			// tick
		if (data & mask)					// Choose bit
		{
			digitalWrite(_data, HIGH);		// Send 1
		}
		else
		{
			digitalWrite(_data, LOW);		// Send 0
		}
		digitalWrite(_clock, HIGH);			// tock
		i--;
	}
}

void LEDTask::max7219Send(unsigned char reg_number, unsigned char column)
{
	digitalWrite(_load, LOW);	// set LOAD 0 to start
	putByte(reg_number);		// Specify row
	putByte(column);			// Specify column
	digitalWrite(_load, 0);		// LOAD 0 to latch
	digitalWrite(_load, 1);		// set LOAD 1 to finish
}

void LEDTask::Init()
{			
	pinMode(_data, OUTPUT);
	pinMode(_clock, OUTPUT);
	pinMode(_load, OUTPUT);
	max7219Send(SCAN_LIMIT, 7);
	max7219Send(DECODE_MODE, 0x00);		// Using LED Matrix (not digits)
	max7219Send(DISPLAY_TEST, 0x00);	// No display test
	max7219Send(INTENSITY, 15);			// High intensity
	max7219Send(SHUTDOWN, 0x00);		// Not in shutdown
	Clear();							// Turn everything off
}

/// <summary>
/// Takes our matrix of rows[] and cols[] and creates 2 bytes and 
/// </summary>
void LEDTask::internalDraw()
{
	unsigned char row = 0;
	unsigned char col = 0;
	for (int a = 0; a <= 7; a++)
	{
		if (rows[a])
		{
			row += 2 ^ a;
		}
		if (cols[a])
		{
			col += 2 ^ a;
		}
	}
	max7219Send(row, col);
}

void LEDTask::Draw(int column, int row, bool powerMode)
{	
	unsigned char mask;
	for (int i = 8; i > 0; i--)
	{
		mask = 0x01 << (i - 1);
		if (column & mask)
		{
			cols[i] = powerMode;			
		}
		if (row & mask)
		{
			rows[i] = powerMode;			
		}
	}
	internalDraw();
}

LEDTask::~LEDTask()
{
	log.log(DEBUG_LOG_LEVEL, "Running ~LEDTask()");
	max7219Send(SHUTDOWN, 0);
	log.log(DEBUG_LOG_LEVEL, "~LEDTask() complete");
}