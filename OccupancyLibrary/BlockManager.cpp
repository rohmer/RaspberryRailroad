#include "BlockManager.h"

BlockManager::BlockManager(Logger logger)
{
	log = logger;
	blockCounter = 0;
	opticalDetector = new OpticalDetector(log);
	taskLib = new TaskLibrary(log);
}

bool BlockManager::AddDetectorArray(ADCPiI2CAddress arrayAddress)
{
	return opticalDetector->AddDetectorArray(arrayAddress);
}

int BlockManager::AddBlock()
{
	log.log(DEBUG_LOG_LEVEL, "Entering BlockManager::AddBlock()");
	Block* newBlock = new Block(log, opticalDetector, taskLib, blockCounter);
	blocks.insert(std::pair<int, Block*>(blockCounter, newBlock));
	int blockNum = blockCounter;
	blockCounter++;
	ostringstream msg;
	msg << "Exiting BlockManager::AddBlock(), rc==" << blockNum;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return blockNum;
}

int BlockManager::AddBlock(std::string blockName)
{
	ostringstream msg;
	msg << "Entering BlockManager::AddBlock(" << blockName << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	Block* newBlock = new Block(log, opticalDetector, taskLib, blockCounter);
	newBlock->SetBlockName(blockName);
	blocks.insert(std::pair<int, Block*>(blockCounter, newBlock));
	int blockNum = blockCounter;
	blockCounter++;
	msg.clear();
	msg << "Exiting BlockManager::AddBlock(), rc==" << blockNum;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return blockNum;
}

int BlockManager::AddBlock(std::string blockName, std::vector<int> activationDetectors, std::vector<int> deactivationDetectors)
{
	ostringstream msg;
	msg << "Entering BlockManager::AddBlock(" << blockName << ", activationDetectors, deactivationDetectors)";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	Block* newBlock = new Block(log, opticalDetector, taskLib, blockCounter);
	newBlock->SetBlockName(blockName);
	for (int a = 0; a < activationDetectors.size(); a++)
		newBlock->AddActivationDetector(activationDetectors[a]);
	for (int a = 0; a < deactivationDetectors.size(); a++)
		newBlock->AddDeactivationDetector(deactivationDetectors[a]);
	blocks.insert(std::pair<int, Block*>(blockCounter, newBlock));
	int blockNum = blockCounter;
	blockCounter++;
	msg.clear();
	msg << "Entering BlockManager::AddBlock(" << blockName << ", activationDetectors, deactivationDetectors), rc=="<<blockNum;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return blockNum;
}

int BlockManager::AddBlock(std::string blockName, std::vector<int> activationDetectors, std::vector<int> deactivationDetectors, std::vector<int> neighborBlocks)
{
	ostringstream msg;
	msg << "Entering BlockManager::AddBlock(" << blockName << ", activationDetectors, deactivationDetectors, neighborBlocks)";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	Block* newBlock = new Block(log, opticalDetector, taskLib, blockCounter);
	newBlock->SetBlockName(blockName);
	for (int a = 0; a < activationDetectors.size(); a++)
		newBlock->AddActivationDetector(activationDetectors[a]);
	for (int a = 0; a < deactivationDetectors.size(); a++)
		newBlock->AddDeactivationDetector(deactivationDetectors[a]);
	for (int a = 0; a < neighborBlocks.size(); a++)
		newBlock->AddNeighbor(neighborBlocks[a]);
	blocks.insert(std::pair<int, Block*>(blockCounter, newBlock));
	int blockNum = blockCounter;
	blockCounter++;
	msg.clear();
	msg << "Entering BlockManager::AddBlock(" << blockName << ", activationDetectors, deactivationDetectors), rc==" << blockNum;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return blockNum;
}

void BlockManager::UpdateBlocks()
{
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Entering BlockManager::UpdateBlocks()"));
	typedef std::map<int, Block*>::iterator it_type;
	for (it_type iterator = blocks.begin(); iterator != blocks.end(); iterator++)
	{
		iterator->second->Update();
	}
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Entering BlockManager::UpdateBlocks()"));
}
