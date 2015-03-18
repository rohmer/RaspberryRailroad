#include "max7219.h"

max7219::max7219(Logger logger)
{
	log = logger;
	spiChannel = 0;
	spiSpeed = 1000000;
	bits = 8;
	device = new char[16];

	ostringstream msg;
	initialized = false;
	msg << "max7219(logger) initialized";
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
	Shutdown(true);
	close(spiFD);
	delete(device);
	log.log(DEBUG_LOG_LEVEL, "max7219::~max7219() completed");
}

void max7219::initialize()
{
	log.log(DEBUG_LOG_LEVEL, "Entering max7219::initialize()");
	ostringstream msg;

	if (spiChannel == 1)
		strcpy(device, "/dev/spidev0.1");
	else
		strcpy(device, "dev/spidev0.0");
	int ret = open(device, O_RDWR);	

	if (spiFD < 0)
	{
		ostringstream msg;
		msg << "Failed to open SPI port " << spiChannel << ".  Try again with sudo";
		log.log(ERROR_LOG_LEVEL, msg.str());
		return;
	}
	
	msg << "Opened file handle: " << spiFD;
	log.log(DEBUG_LOG_LEVEL, msg.str());

	ret = ioctl(spiFD, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		log.log(ERROR_LOG_LEVEL,"can't set spi mode");

	ret = ioctl(spiFD, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		log.log(ERROR_LOG_LEVEL,"can't get spi mode");

	/** bits per word **/
	ret = ioctl(spiFD, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		log.log(ERROR_LOG_LEVEL,"can't set bits per word");

	ret = ioctl(spiFD, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		log.log(ERROR_LOG_LEVEL,"can't get bits per word");

	/** max speed hz **/
	ret = ioctl(spiFD, SPI_IOC_WR_MAX_SPEED_HZ, spiSpeed);
	if (ret == -1)
		log.log(ERROR_LOG_LEVEL,"can't set max speed hz");

	ret = ioctl(spiFD, SPI_IOC_RD_MAX_SPEED_HZ, spiSpeed);
	if (ret == -1)
		log.log(ERROR_LOG_LEVEL,"can't get max speed hz");
	
	Clear();

	log.log(DEBUG_LOG_LEVEL, "Exiting max7219::initialize()");
	initialized = true;
}

void max7219::SetIntensity(int value)
{
	ostringstream msg;
	ostringstream args;
	args << value;
	msg << "Entering max7219::SetIntensity(" << value << ")";
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
		spiTransfer(INTENSITY, value);
	else
		spiTransfer(INTENSITY, value);
	msg.clear();
	msg << "Exiting max7219::SetIntensity(" << value << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());

}

void max7219::Shutdown(bool value)
{
	ostringstream msg;
	ostringstream args;
	args << value;
	msg << "Entering max7219::Shutdown(" << args<< ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (value)
		spiTransfer(SHUTDOWN, 1);
	else
		spiTransfer(SHUTDOWN, 0);
}

void max7219::Clear()
{
	log.log(DEBUG_LOG_LEVEL,"Entering max7219::Clear()");
	for (int col = 1; col <= 8; col++) { spiTransfer(col, 0); }
	log.log(DEBUG_LOG_LEVEL, "Exiting max7219::Clear()");
}

void max7219::spiTransfer(unsigned char opcode, unsigned char data)
{
	ostringstream msg;
	ostringstream args;
	args << opcode << "," << data;

	msg << "Entering max7219::spiTransfer(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());

	uint8_t tx[] = { opcode, data, };
	int ret=write(spiFD, tx, 2);
	if (ret==-1)
	{
		msg.clear();
		msg << "Error writing to device: " << spiFD;
		log.log(ERROR_LOG_LEVEL, msg.str());
	}
	else
	{
		msg.clear();
		msg << "Wrote: " << opcode << data << " to SPI channel: " << spiChannel << " successfully";
		log.log(DEBUG_LOG_LEVEL, msg.str());
	}

	msg.clear();
	msg << "Exiting max7219::spiTransfer(" << args << ")";
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
		log.log(DEBUG_LOG_LEVEL, "Exiting max7219::Draw()");		
		return;
	}

	status[(row - 1) * 8 + col] = powerMode;

	uint8_t val;

	for (int col = 0; col < 8; col++)
	{
		val = 0;
		for (int row = 0; row < 8; row++)
		{
			if (status[(row * 8) + col])
			{
				val += pow(2, col);
			}
		}
		spiTransfer(col + 1, val);
	}
	msg.clear();
	msg << "Exiting max7219::Draw(" << args << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

