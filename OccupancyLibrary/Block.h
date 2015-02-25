#pragma once
#include "log4cplus\logger.h"
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <errno.h>
#include "DetectorBase.h"
#include "OpticalDetector.h"
#include "ADCPI.h"

class Block
{
	private:
		vector<int> activationDetectors;
		vector<int> connectedBlocks;
		vector<int> deactivationDetectors;

		bool isOccupied;
		bool neighborOccupied;
		std::string blockName;

		OpticalDetector *opticalDetector;

		Logger log;
		int blockID;

	public:
		Block(Logger logger, OpticalDetector* opticalDetector, int blockIdentifier);
		void AddActivationDetector(int detectorNum);
		void AddDeactivationDetector(int detectorNum);
		void Update();
		void NeighborOccupied(bool value);
		bool IsNeighborOccupied();
		bool IsOccupied();
		void SetBlockName(std::string value);
		void AddNeighbor(int neighbor);
		std::string GetBlockName();
};
