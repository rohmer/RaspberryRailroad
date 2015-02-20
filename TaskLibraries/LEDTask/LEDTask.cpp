#include "LEDTask.h"

LEDTask::LEDTask(Logger logger) :TaskBase(logger)
{	
	log.log(log4cplus::DEBUG_LOG_LEVEL, "LEDTask::LEDTask created");
	_ce1 = 11;
	_ce0 = 10;
	_sclk = 14;
	_miso = 13;
	_mosi = 12;
	_rxd = 16;
	_txd = 15;
	_scl = 9;
	_sda = 8;
	SetPins(_ce1, _ce0, _sclk, _miso, _mosi, _rxd, _txd, _scl, _sda);
	numDevice = 1;
}

void LEDTask::Init()
{		
	for (int a = 0; a < numDevice; a++)
	{
		int rc = wiringPiSPISetup(a, 1000000);

			if (rc == 0)
			{
				ostringstream msg;
				msg << "wiringPiSetup initalized successfully, channel: " << a;

				log.log(log4cplus::DEBUG_LOG_LEVEL, msg.str());
				successfulInit = true;
			}
			else
			{
				ostringstream msg;
				msg << "wiringPiSetup failed to initalize, on channel: " << a << ", rc==" << rc;
				log.log(log4cplus::ERROR_LOG_LEVEL, msg.str());
				successfulInit = false;
			}
	}
}

void LEDTask::SetPins(int ce1, int ce0, int sclk, int miso, int mosi, int rxd, int txd, int scl, int sda)
{
	_ce1 = ce1;
	_ce0 = ce0;
	_sclk = sclk;
	_miso = miso;
	_mosi = mosi;
	_rxd = rxd;
	_txd = txd;
	_scl = scl;
	_sda = sda;
	rows[0]= 0;
	rows[1] = 5;
	rows[2] = _scl;
	rows[3] = 3;
	rows[4] = _ce1;
	rows[5] = _txd;
	rows[6] = _ce0;
	rows[7] = _mosi;
	columns[0] = 4;
	columns[1] = _sclk;
	columns[2] = _miso;
	columns[3] = 1;
	columns[4] = _rxd;
	columns[5] = 2;
	columns[6] = 6;
	columns[7] = 7;
}

void LEDTask::Clear()
{
	for (int col = 0; col < 8; col++)
	{
		digitalWrite(columns[col], LOW);
		for (int row = 0; row < 8; row++)
		{
			digitalWrite(rows[row], LOW);
		}
	}
}

void LEDTask::SetNumDevice(int numberOfMax)
{
	numDevice = numberOfMax;
	log.log(DEBUG_LOG_LEVEL, "numDevice set to: " + numDevice);
}

void LEDTask::Draw(int column, int row, bool powerMode)
{
	int val;
	if (powerMode)
		val = 1;
	else
		val = 0;

	for (int col = 0; col < 8; col++)
		if (col != column)
			digitalWrite(columns[col], val);
	digitalWrite(rows[row], val);
}

LEDTask::~LEDTask()
{
	log.log(DEBUG_LOG_LEVEL, "Running ~LEDTask()");
	for (int i = 0; i < 8; i++)
	{
		pinMode(rows[i], INPUT);
		pinMode(columns[i], INPUT);
	}
	log.log(DEBUG_LOG_LEVEL, "~LEDTask() complete");
}