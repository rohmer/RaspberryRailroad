#include "max7219.h"

max7219::max7219(Logger logger, int numberOfDevices)
{
	log = logger;
	spiChannel = 0;
	numOfDevices = numberOfDevices;
	if (numOfDevices < 1)
	{
		log.log(WARN_LOG_LEVEL, "numberOfDevices < 1, defaulting to 1");
		numOfDevices = 1;
	}
	spiSpeed = 1000000;
	char initArray[80];
	for (int a = 0; a++; a < 10)
	{
		initArray[a * 10] = 0;
		initArray[a * 10 + 1] = 'b';
	}

	for (int a = 0; a < numOfDevices; a++)
	{
		maxData.insert(std::pair<int, char[80]>(a, initArray));
	}

	TxBuffer = new char[1024];
	RxBuffer = new char[1024];
	TxBufferIndex = 0;
	RxBufferIndex = 0;
	ostringstream msg;
	initialized = false;
	msg << "max7219(logger," << numberOfDevices << ") initialized";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void max7219::SetNumDevices(int val)
{
	ostringstream msg;
	msg << "Entering max7219::SetNumDevices(" << val << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	numOfDevices = val;
	maxData.clear();
	char initArray[80];
	for (int a = 0; a++; a < 10)
	{
		initArray[a * 10] = 0;
		initArray[a * 10 + 1] = 'b';
	}

	for (int a = 0; a < numOfDevices; a++)
	{
		maxData.insert(std::pair<int, char[80]>(a, initArray));
	}
	msg << "Exiting max7219::SetNumDevices(" << val << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());

}
void max7219::SetSPIChannel(int val)
{
	ostringstream msg;
	msg << "Entering max7219::SetSPIChannel(" << val << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	spiChannel = val;
	if (initialized)
	{
		close(spiFD);
		initialized = false;		
	}
	initialize();
	msg.clear();
	msg << "Exiting max7219::SetSPIChannel(" << val << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void max7219::SetSPISpeed(int val)
{
	ostringstream msg;
	msg << "Entering max7219::SetSPISpeed(" << val << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	spiSpeed = val;
	if (initialized)
	{
		close(spiFD);
		initialized = false;
	}
	initialize();
	msg.clear();
	msg << "Exiting max7219::SetSPISpeed(" << val << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}


max7219::~max7219()
{
	SetShutDown(true);
	delete[] TxBuffer;
	delete[] RxBuffer;
	close(spiFD);
	log.log(DEBUG_LOG_LEVEL, "max7219::~max7219() completed");
}

void max7219::initialize()
{
	log.log(DEBUG_LOG_LEVEL, "Entering max7219::initialize()");
	ostringstream msg;

	if ((spiFD = wiringPiSPISetup(spiChannel, spiSpeed) < 0))
	{
		ostringstream msg;
		msg << "Failed to open SPI port " << spiChannel << ".  Try again with sudo";
		log.log(ERROR_LOG_LEVEL, msg.str());
		return;
	}
	msg << "Opened file handle: " << spiFD;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	SetDecodeMode(false);
	SetScanLimit(7);
	SetIntensity(15);
	SetShutDown(false);

	log.log(DEBUG_LOG_LEVEL, "Exiting max7219::initialize()");
	initialized = true;
}

void max7219::transfer(char c)
{
	ostringstream msg;
	msg << "Entering max7219::transfer(" << c << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());

	TxBuffer[TxBufferIndex] = c;
	TxBufferIndex++;
	log.log(DEBUG_LOG_LEVEL,"Exiting max7219::transfer()");
}

void max7219::endTransfer()
{
	if (!initialized)
	{
		initialize();
	}
	if (!initialized)
	{
		log.log(DEBUG_LOG_LEVEL, "Exiting max7219::endTransfer()");
		TxBufferIndex = 0;
		return;
	}
	log.log(DEBUG_LOG_LEVEL, "Entering max7219::endTransfer()");
	int temp = write(spiFD, TxBuffer, TxBufferIndex);
	ostringstream msg;
	msg << "Written: " << temp << " Index: " << TxBufferIndex << " Buffer: ";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	TxBufferIndex = 0;
	log.log(DEBUG_LOG_LEVEL, "Exiting max7219::endTransfer()");
}

void max7219::setData(char row, char data, int device)
{
	ostringstream msg;
	ostringstream args;	
	args << row << "," << data << "," << device;
	msg << "Entering max7219::setData(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	for (int i = numOfDevices; i > 0; i--)
	{
		if ((i == device) || (device==255))
		{
			transfer(row);
			transfer(data);
		}
		else
		{
			transfer(NoOp);
			transfer(0);
		}
	}
	endTransfer();
	delay(1);

	msg.clear();
	msg << "Exiting max7219::setData(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void max7219::setData(char row, char data)
{
	setData(row, data, 255);
}

void max7219::Clear(int maxDevice)
{
	ostringstream msg;
	msg << "Entering max7219::Clear(" << maxDevice << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	for (int row = 1; row <= 8; row++)
		for (int col = 1; col <= 8; col++)
		{
			int pt = (row - 1) * 10 + (col - 1) + 2;
			maxData[maxDevice].data[pt] = '0';
		}
	for (int a = 0; a < 8; a++)
	{
		setData(a + 1, char(maxData[maxDevice].data[a]), maxDevice);
	}
	msg.clear();
	msg << "Exiting max7219::Clear(" << maxDevice << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void max7219::Clear()
{
	log.log(DEBUG_LOG_LEVEL,"Entering max7219::Clear())");
	for (int a = 0; a < numOfDevices; a++)
	{
		Clear(a);
	}
	log.log(DEBUG_LOG_LEVEL, "Exiting max7219::Clear())");
}

void max7219::Draw(int row, int col, int maxDevice, bool powerMode)
{
	ostringstream msg;
	ostringstream args;
	args << row << "," << col << "," << maxDevice << "," << powerMode;
	msg << "Entering max7219::Draw(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());

	// find the point in our array to set;
	char val;
	if (powerMode)
	{
		val = '1';
	}
	else
	{
		val = '0';
	}
	int pt = (row - 1) * 10 + (col-1) + 2;
	maxData[maxDevice].data[pt] = val;
	for (int a = 0; a < 8; a++)
	{
		setData(a + 1, char(maxData[maxDevice].data[a]), maxDevice);
	}
	msg.clear();
	msg << "Entering max7219::Draw(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void max7219::Draw(int row, int col, bool powerMode)
{
	ostringstream msg;
	ostringstream args;
	args << row << "," << col << "," << powerMode;
	msg << "Entering max7219::Draw(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	for (int a = 0; a < numOfDevices; a++)
	{
		char val;
		if (powerMode)
		{
			val = '1';
		}
		else
		{
			val = '0';
		}
		int pt = (row - 1) * 10 + col + 2;
		maxData[a].data[pt] = val;
	}

	for (int b = 0; b < numOfDevices; b++)
	{
		for (int a = 0; a < 8; a++)
		{
			setData(a + 1, char(maxData[b].data[a]), b);
		}
	}
	msg.clear();
	msg << "Entering max7219::Draw(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}