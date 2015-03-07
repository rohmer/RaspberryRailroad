#include "Block.h"

// Block Manager will call this, and assign a unique blockIdenifier
Block::Block(Logger logger, OpticalDetector* opticalDetector, TaskLibrary *taskLibrary, int blockIdentifier)
{
	log = logger;
	isOccupied = false;
	neighborOccupied = false;
	this->opticalDetector = opticalDetector;
	blockID = blockIdentifier;
	taskLib = taskLibrary;
	log.log(DEBUG_LOG_LEVEL, "Block::Block() initalized");
}

Block::Block(Logger logger, int blockIdentifier)
{
	log = logger;
	isOccupied = false;
	neighborOccupied = false;
	blockID = blockIdentifier;
	log.log(DEBUG_LOG_LEVEL, "Block::Block() initalized");
}

void Block::AddDetector(int detectorNum)
{
	ostringstream msg;
	msg << "Adding Detector: " << detectorNum;
	log.log(DEBUG_LOG_LEVEL, msg.str());
	detectors.push_back(detectorNum);
}

vector<int> Block::GetNeighbors()
{
	return connectedBlocks;
}

void Block::Update()
{
	ostringstream msg;
	msg << "Entering Block::Update() for blockID: " << blockID;
	log.log(DEBUG_LOG_LEVEL,msg.str());	
	if(!isOccupied)
	{
		for (int a = 0; a < detectors.size(); a++)
		{
			opticalDetector->UpdateDetector(detectors[a]);
			if (opticalDetector->IsTriggered(detectors[a]))
			{
				msg.clear();
				msg << "Block :" << blockID << " is becoming occupied";
				log.log(DEBUG_LOG_LEVEL, msg.str());
				isOccupied = true;
				for (int b = 0; b < activationTasks.size(); b++)
					execTask(activationTasks[a]);
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
	return neighborOccupied;
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

/// <summary>
/// Creates a block task.
/// </summary>
/// <param name="taskType">Type of the task.</param>
/// <param name="runArguments">The passed to the task</param>
/// <param name="">Continuation of args passed to the task</param>
/// <returns>BlockTaskType struct</returns>
Block::BlockTaskType Block::CreateBlockTask(TaskTypes taskType, int runArguments, ...)
{
	ostringstream msg;
	msg << "Entering Block::CreateBlockTask(" << taskType;
	va_list args;
	va_start(args, runArguments);
	vector<int> runArgs;
	for (int i = 1; i < runArguments; i++)
	{
		int val = va_arg(args, int);
		msg << ", " << val;
		runArgs.push_back(val);
	}
	msg << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	BlockTaskType btt;
	btt.runArgs = runArgs;
	btt.taskType = taskType;
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Exiting Block::CreateBlockTask()"));
	return btt;
}

void Block::AddActivationTask(BlockTaskType task)
{
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Entering Block::AddActivationTask()"));
	activationTasks.push_back(task);
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Exiting Block::AddActivationTask()"));
}

void Block::AddDeactivationTask(BlockTaskType task)
{
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Entering Block::AddDeactivationTask()"));
	deactivationTasks.push_back(task);
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Exiting Block::AddDeactivationTask()"));
}

/// <summary>
/// Executes the task within the taskLibrary.
/// </summary>
/// <param name="task">The task previously defined</param>
void Block::execTask(BlockTaskType task)
{
	log.log(DEBUG_LOG_LEVEL,LOG4CPLUS_TEXT("Entering Block::execTask()"));
	TaskBase* t=taskLib->GetTask(task.taskType);
	t->Run(task.runArgs);
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Exiting Block::execTask()"));
}

int Block::GetID()
{
	return blockID;
}