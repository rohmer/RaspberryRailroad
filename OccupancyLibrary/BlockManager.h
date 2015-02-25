#pragma	once
#include <map>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <errno.h>
#include "log4cplus\logger.h"
#include "Block.h"
#include "ADCPI.h"
#include "DetectorBase.h"
#include "OpticalDetector.h"

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

	public:
		BlockManager(Logger logger);
		bool AddDetectorArray(ADCPiI2CAddress arrayAddress);
		int AddBlock();
		int AddBlock(std::string blockName);
		int AddBlock(std::string blockName, std::vector<int> activationDetectors, std::vector<int> deactivationDetectors);
		int AddBlock(std::string blockName, std::vector<int> activationDetectors, std::vector<int> deactivationDetectors, std::vector<int> neighborBlocks);

		void UpdateBlocks();



};