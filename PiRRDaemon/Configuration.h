#pragma once

#include <log4cplus/logger.h>
#include <libjson.h>
#include <fstream>
#include <string>
#include <streambuf>
#include <>
using namespace std;
using namespace log4cplus;

class Configuration
{
	private:
		Logger logger;
		string configFile;
		bool configParsed;
		
		bool logToStdOut, logToNet, logToFile;
		string logFilename,logServer;
		int logPort;
	
		bool parseConfig();
		void parseJSON(const JSONNODE *n);
	
	public:
		Configuration();
		Configuration(string configFile);
};