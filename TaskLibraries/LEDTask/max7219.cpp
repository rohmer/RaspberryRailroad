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
	initialized = false;
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
	msg.clear();
	msg << "Exiting max7219::SetSPISpeed(" << val << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}


max7219::~max7219()
{
	for (int a = 0; a < numOfDevices; a++)
		Shutdown(a, true);
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
	for (int i = 0; i < numOfDevices; i++)
	{
		spiTransfer(i, DisplayTest, 0);
		spiTransfer(i, ScanLimit, 7);
		spiTransfer(i, DecodeMode, 0);
		SetIntensity(i, 15);

	}
	
	log.log(DEBUG_LOG_LEVEL, "Exiting max7219::initialize()");
	initialized = true;
}

void max7219::SetIntensity(int addr, int value)
{
	ostringstream msg;
	ostringstream args;
	args << addr << "," << value;
	msg << "Entering max7219::SetIntensity(" << addr << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (value > 15)
	{
		log.log(WARN_LOG_LEVEL, "Intensity set to >15, defaulting to 15");
		value = 15;
	}
	if (value < 0)
	{
		log.log(WARN_LOG_LEVEL, "Intensity set to <0, defaulting to 0");
		value = 0;
	}
	if (value)
		spiTransfer(addr, Intensity, value);
	else
		spiTransfer(addr, Intensity, value);
}

void max7219::Shutdown(int addr, bool value)
{
	ostringstream msg;
	ostringstream args;
	args << addr << "," << value;
	msg << "Entering max7219::Shutdown(" << addr << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (value)
		spiTransfer(addr, ShutDown, 1);
	else
		spiTransfer(addr, ShutDown, 0);
}

void max7219::Clear(int addr)
{
	ostringstream msg;
	msg << "Entering max7219::Clear(" << addr << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	int offset;
	if (addr<0 || addr>numOfDevices)
	{
		log.log(ERROR_LOG_LEVEL, "Address is outside of the range of max Devices");
		msg.clear();
		msg << "Exiting max7219::Clear(" << addr << ")";
		log.log(DEBUG_LOG_LEVEL, msg.str());
	}

	offset = addr * 8;
	for (int i = 0; i < 8; i++)
	{
		status[offset + i] = 0;
		spiTransfer(addr, i + 1, status[offset + i]);
	}
	msg << "Exiting max7219::Clear(" << addr << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void max7219::Clear()
{
	log.log(DEBUG_LOG_LEVEL,"Entering max7219::Clear()");
	for (int i = 0; i < numOfDevices; i++)
		Clear(i);
	log.log(DEBUG_LOG_LEVEL, "Exiting max7219::Clear()");
}

void max7219::spiTransfer(int addr, volatile unsigned char opcode, volatile unsigned char data)
{
	ostringstream msg;
	ostringstream args;
	args << addr << "," << opcode << "," << data;
	msg << "Entering max7219::spiTransfer(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());

	int offset = addr * 2;
	int maxbytes = numOfDevices * 2;

	for (int i = 0; i < maxbytes; i++)
		spiData[i] = (char)0;

	spiData[offset + 1] = opcode;
	spiData[offset] = data;
	int bytesWritten = wiringPiSPIDataRW(spiChannel, spiData, maxbytes);
	if (bytesWritten != maxbytes)
	{
		msg.clear();
		msg << "Expected to write: " << maxbytes << ", actually wrote: " << bytesWritten;
		log.log(ERROR_LOG_LEVEL, msg.str());
	}
	else
	{
		msg.clear();
		msg << "Wrote: " << bytesWritten << " to SPI channel: " << spiChannel << " successfully";
		log.log(DEBUG_LOG_LEVEL, msg.str());
	}

	msg.clear();
	msg << "Exiting max7219::spiTransfer(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}


void max7219::Draw(int row, int col, int maxDevice, bool powerMode)
{
	ostringstream msg;
	ostringstream args;
	args << row << "," << col << "," << maxDevice << "," << powerMode;
	msg << "Entering max7219::Draw(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (!initialized)
	{
		initialize();
	}
	if (!initialized)
	{
		log.log(DEBUG_LOG_LEVEL, "Exiting max7219::Draw()");		
		return;
	}

	int offset;
	char val = 0x00;
	if (maxDevice<0 || maxDevice>numOfDevices)
	{
		log.log(ERROR_LOG_LEVEL, "maxDevice is outside the range of max devices, not drawing");
		msg.clear();
		msg << "Exiting max7219::Draw(" << args << ")";
		log.log(DEBUG_LOG_LEVEL, msg.str());
	}

	offset = maxDevice * 8;
	val = 128 >> col;
	if (powerMode)
		status[offset + row] = status[offset + row] | val;
	else
	{
		val = ~val;
		status[offset + row] = status[offset + row] & val;
	}
	spiTransfer(maxDevice, row + 1, status[offset + row]);
	msg.clear();
	msg << "Exiting max7219::Draw(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void max7219::Draw(int row, int col, bool powerMode)
{
	ostringstream msg;
	ostringstream args;
	args << row << "," << col << "," << powerMode;
	msg << "Entering max7219::Draw(" << args << ")";

	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (!initialized)
	{
		initialize();
	}
	if (!initialized)
	{
		log.log(DEBUG_LOG_LEVEL, "Exiting max7219::endTransfer()");		
		return;
	}

	for (int a = 0; a < numOfDevices; a++)
	{
		Draw(row, col, a, powerMode);
	}

	msg.clear();
	msg << "Exiting max7219::Draw(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}