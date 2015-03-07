#pragma once
#include "log4cplus\logger.h"
#include <vector>
#include "BlockManager.h"
#include "BlockHelper.h"
#include "Block.h"

using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace std;

// Base class of the occupancy library
class OccupancyLibrary
{
	private:
		Logger log;
		BlockManager* blockManager;
		TaskLibrary* taskLibrary;

	public:
		OccupancyLibrary();
		OccupancyLibrary(log4cplus::LogLevel minSevToLog, bool logToStdOut, bool logToFile, std::string filename, bool logToNet, int port, std::string hostname);
		OccupancyLibrary(Logger logger);
		bool ImportXML(string xmlFilename);
		Block* GetBlock(string blockName);
		Logger GetLogger() { return log; }
};