#pragma once
#include "log4cplus/logger.h"
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <errno.h>
#include "DetectorBase.h"
#include "OpticalDetector.h"
#include "TaskBase/TaskBase.h"
#include "TaskLibrary.h"
#include <stdarg.h>
#include "ADCPI.h"
#include <stdio.h>

class Block
{
	struct BlockTaskType
	{
		TaskTypes taskType;
		vector<int> runArgs;
	};

	private:
		vector<int> activationDetectors;
		vector<int> connectedBlocks;
		vector<int> deactivationDetectors;		
		vector<BlockTaskType> activationTasks;
		vector<BlockTaskType> deactivationTasks;

		bool isOccupied;
		bool neighborOccupied;
		std::string blockName;

		TaskLibrary *taskLib;
		OpticalDetector *opticalDetector;		
		Logger log;
		int blockID;

		void execTask(BlockTaskType task);

	public:
		Block(Logger logger, OpticalDetector* opticalDetector, TaskLibrary* taskLibrary, int blockIdentifier);
		void AddActivationDetector(int detectorNum);
		void AddDeactivationDetector(int detectorNum);
		void Update();
		void NeighborOccupied(bool value);
		bool IsNeighborOccupied();
		bool IsOccupied();
		void SetBlockName(std::string value);
		void AddNeighbor(int neighbor);
		std::string GetBlockName();
		BlockTaskType CreateBlockTask(TaskTypes taskType, int runArguments, ...);
		void AddActivationTask(BlockTaskType task);
		void AddDeactivationTask(BlockTaskType task);
};
