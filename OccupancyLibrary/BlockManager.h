#pragma	once
#include <map>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <errno.h>
#include "log4cplus\logger.h"
#include "Block.h"
#include "ADCPI.h"
#include "OpticalDetector.h"
#include "BlockHelper.h"

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

class BlockManager
{
	private:
		map<int, Block*> blocks;
		int blockCounter;
		Logger log;
		OpticalDetector* opticalDetector;
		TaskLibrary* taskLib;

	public:
		BlockManager(Logger logger);
		bool AddDetectorArray(ADCPiI2CAddress arrayAddress);
		int AddBlock();
		int AddBlock(std::string blockName);
		int AddBlock(std::string blockName, std::vector<int> deectors);
		int AddBlock(std::string blockName, std::vector<int> detectors, std::vector<int> neighborBlocks);
		int AddBlock(Block* block);

		void UpdateBlocks();

		Block* GetBlock(string blockName);


};