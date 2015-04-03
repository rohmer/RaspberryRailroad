#pragma once

#include <log4cplus/logger.h>
#include <libjson.h>
#include <fstream>
#include <string>
#include <streambuf>

using namespace std;
using namespace log4cplus;

class Configuration
{
	private:
		Logger logger;
		string configFile;
		bool configParsed;

		bool parseConfig();

	public:
		Configuration();
		Configuration(string configFile);
};