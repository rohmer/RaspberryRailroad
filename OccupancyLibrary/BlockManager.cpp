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

int BlockManager::AddBlock(Block* block)
{
	log.log(DEBUG_LOG_LEVEL, "Entering BlockManager::AddBlock()");

	blocks.insert(std::pair<int, Block*>(block->GetID(), block));

	ostringstream msg;
	msg << "Exiting BlockManager::AddBlock(), rc==" << block->GetID();
	log.log(DEBUG_LOG_LEVEL, msg.str());
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

int BlockManager::AddBlock(std::string blockName, std::vector<int> detectors)
{
	ostringstream msg;
	msg << "Entering BlockManager::AddBlock(" << blockName << ", detectors)";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	Block* newBlock = new Block(log, opticalDetector, taskLib, blockCounter);
	newBlock->SetBlockName(blockName);
	for (int a = 0; a < detectors.size(); a++)
		newBlock->AddDetector(detectors[a]);
	blocks.insert(std::pair<int, Block*>(blockCounter, newBlock));
	int blockNum = blockCounter;
	blockCounter++;
	msg.clear();
	msg << "Entering BlockManager::AddBlock(" << blockName << ", activationDetectors, deactivationDetectors), rc=="<<blockNum;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return blockNum;
}

int BlockManager::AddBlock(std::string blockName, std::vector<int> detectors, std::vector<int> neighborBlocks)
{
	ostringstream msg;
	msg << "Entering BlockManager::AddBlock(" << blockName << ", detectors, neighborBlocks)";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	Block* newBlock = new Block(log, opticalDetector, taskLib, blockCounter);
	newBlock->SetBlockName(blockName);
	for (int a = 0; a < detectors.size(); a++)
		newBlock->AddDetector(detectors[a]);
	for (int a = 0; a < neighborBlocks.size(); a++)
		newBlock->AddNeighbor(neighborBlocks[a]);
	blocks.insert(std::pair<int, Block*>(blockCounter, newBlock));
	int blockNum = blockCounter;
	blockCounter++;
	msg.clear();
	msg << "Entering BlockManager::AddBlock(" << blockName << ", detectors, neighborBlocks), rc==" << blockNum;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	return blockNum;
}

void BlockManager::UpdateBlocks()
{
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Entering BlockManager::UpdateBlocks()"));
	typedef std::map<int, Block*>::iterator it_type;
	for (it_type iterator = blocks.begin(); iterator != blocks.end(); iterator++)
	{
		// First check neighbors to see if they are active
		for (int n = 0; n < iterator->second->GetNeighbors().size(); n++)
		{
			Block* neighbor = blocks[iterator->second->GetNeighbors()[n]];
			if (neighbor->IsOccupied())
			{
				// Neighbor is occupied, so we arent
				iterator->second->SetOccupied(false);
				ostringstream msg;
				msg << "Setting block: " << iterator->first << " to unoccupied because neighbor is occupied";
				log.log(DEBUG_LOG_LEVEL, msg.str());
			}
		}
		iterator->second->Update();
	}
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Entering BlockManager::UpdateBlocks()"));
}

Block* BlockManager::GetBlock(string blockName)
{
	typedef std::map<int, Block*>::iterator it_type;
	for (it_type iterator = blocks.begin(); iterator != blocks.end(); iterator++)
	{
		if (iterator->second->GetBlockName().compare(blockName) == 0)
			return iterator->second;
	}

	return NULL;
}