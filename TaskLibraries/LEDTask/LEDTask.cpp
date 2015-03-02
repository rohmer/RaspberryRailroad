#include "LEDTask.h"

LEDTask::LEDTask(Logger logger) :TaskBase(logger)
{	
	log.log(log4cplus::DEBUG_LOG_LEVEL, "LEDTask::LEDTask created");	
	_spiChannel = 0;
	_spiSpeed = 1000000;
	_maxCount = 1;
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
	if (!successfulInit)
		Init();
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

void LEDTask::Init()
{			
	spiFD = max7219_init(_spiChannel, _spiSpeed, _maxCount);
	max7219_set_intensity(0, 15);
	max7219_set_decode(0, NO_DECODE);
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
	max7219_send_data(0, row, col);
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
	max7219_set_shutdown(0, SHUTDOWN_MODE);
	log.log(DEBUG_LOG_LEVEL, "~LEDTask() complete");
}