#pragma once
#include "pugiconfig.hpp"
#include "pugixml.hpp"
#include <map>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <errno.h>
#include "log4cplus\logger.h"
#include "Block.h"


using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace pugi;

// Class for reading XML and creating a blockmap for the BlockManager
class BlockHelper
{
	public:
		static vector<Block*> ReadXMLBlockFile(Logger logger, std::string filename);
		static Block* GetBlockByName(vector<Block*> input, std::string name);
};