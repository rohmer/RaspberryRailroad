#include "OccupancyLibrary.h"

OccupancyLibrary::OccupancyLibrary()
{
	taskLibrary = new TaskLibrary(WARN_LOG_LEVEL, true, false, "", false, 0, "");
	log = taskLibrary->GetLogger();
	blockManager = new BlockManager(log);
	log.log(DEBUG_LOG_LEVEL, "OccupancyLibrary initalized");
}

OccupancyLibrary::OccupancyLibrary(log4cplus::LogLevel minSevToLog, bool logToStdOut, bool logToFile, std::string filename, bool logToNet, int port, std::string hostname)
{
	taskLibrary = new TaskLibrary(minSevToLog, logToStdOut, logToFile, filename, logToNet, port, hostname);
	log = taskLibrary->GetLogger();
	blockManager = new BlockManager(log);
	log.log(DEBUG_LOG_LEVEL, "OccupancyLibrary initalized");
}

OccupancyLibrary::OccupancyLibrary(Logger logger)
{
	taskLibrary = new TaskLibrary(logger);
	log = logger;
	blockManager = new BlockManager(log);
	log.log(DEBUG_LOG_LEVEL, "OccupancyLibrary initalized");
}

bool OccupancyLibrary::ImportXML(string xmlFilename)
{
	ostringstream msg;
	msg << "Entering OccupancyLibrary::ImportXML(" << xmlFilename << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	vector<Block*> blocks = BlockHelper::ReadXMLBlockFile(log, xmlFilename);
	if (blocks.size() == 0)
	{
		log.log(WARN_LOG_LEVEL, "No blocks read, error condition?");
		msg.clear();
		msg << "Exiting OccupancyLibrary::ImportXML(" << xmlFilename << ")";
		log.log(DEBUG_LOG_LEVEL, msg.str());
		return false;
	}
	for (int a = 0; a < blocks.size(); a++)
	{
		blockManager->AddBlock(blocks[a]);
	}
	msg.clear();
	msg << "Exiting OccupancyLibrary::ImportXML(" << xmlFilename << "), rc==true";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return true;
}

Block* OccupancyLibrary::GetBlock(string blockName)
{
	return blockManager->GetBlock(blockName);
}