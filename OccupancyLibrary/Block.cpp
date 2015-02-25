#include "Block.h"

// Block Manager will call this, and assign a unique blockIdenifier
Block::Block(Logger logger, OpticalDetector* opticalDetector, int blockIdentifier)
{
	log = logger;
	isOccupied = false;
	neighborOccupied = false;
	this->opticalDetector = opticalDetector;
	blockID = blockIdentifier;
	
	log.log(DEBUG_LOG_LEVEL, "Block::Block() initalized");
}

void Block::AddActivationDetector(int detectorNum)
{
	ostringstream msg;
	msg << "Adding activation point: " << detectorNum;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	activationDetectors.push_back(detectorNum);
}

void Block::AddDeactivationDetector(int detectorNum)
{
	ostringstream msg;
	msg << "Adding deactivation point: " << detectorNum;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	deactivationDetectors.push_back(detectorNum);
}

void Block::Update()
{
	ostringstream msg;
	msg << "Entering Block::Update() for blockID: " << blockID;
	log.log(DEBUG_LOG_LEVEL,msg.str());
	// Only process potential state changes
	// i.e. Occupied -> Unoccupied
	if (isOccupied)
	{
		for (int a = 0; a < deactivationDetectors.size(); a++)
		{
			opticalDetector->UpdateDetector(deactivationDetectors[a]);
			if (opticalDetector->IsTriggered)
			{
				msg.clear();
				msg << "Block :" << blockID << " is becoming unoccupied";
				log.log(DEBUG_LOG_LEVEL, msg.str());
				isOccupied = false;			
			}
		}
	}
	else
	{
		for (int a = 0; a < activationDetectors.size(); a++)
		{
			opticalDetector->UpdateDetector(activationDetectors[a]);
			if (opticalDetector->IsTriggered)
			{
				msg.clear();
				msg << "Block :" << blockID << " is becoming occupied";
				log.log(DEBUG_LOG_LEVEL, msg.str());
				isOccupied = true;
			}
		}
	}
	msg << "Exiting Block::Update() for blockID: " << blockID;
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void Block::NeighborOccupied(bool value)
{
	neighborOccupied = value;
}

bool Block::IsNeighborOccupied()
{
	return IsNeighborOccupied;
}

bool Block::IsOccupied()
{
	return isOccupied;
}

void Block::SetBlockName(std::string value)
{
	blockName = value;
}

std::string Block::GetBlockName()
{
	return blockName;
}

void Block::AddNeighbor(int neighbor)
{
	bool exists = false;
	for (int a = 0; a < connectedBlocks.size(); a++)
		if (connectedBlocks[a] == neighbor)
			exists = true;
	if (exists)
	{
		ostringstream msg;
		msg << "Not adding neighbor: " << neighbor << ", already exists for block Id: " << blockID;
		log.log(DEBUG_LOG_LEVEL, msg.str());
		return;
	}
	ostringstream msg;
	msg << "Added neighbor: " << neighbor << ", for block Id: " << blockID;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	connectedBlocks.push_back(neighbor);
	return;
}