#pragma once

#include <log4cplus\logger.h>
#include <log4cplus\consoleappender.h>
#include <log4cplus\fileappender.h>
#include <log4cplus\socketappender.h>
#include <log4cplus/ndc.h>
#include <log4cplus/mdc.h>
#include <log4cplus/layout.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

class LogHelper
{
	private:

	public:
		static Logger GetLogger(log4cplus::LogLevel minSevToLog, bool logToStdOut, 
								bool logToFile, std::string logFile, 
								bool logToNet, int port, std::string hostname);
};