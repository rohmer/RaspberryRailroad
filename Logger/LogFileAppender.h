#pragma once
#include "Appender.h"
#include "Common.h"
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef __cplusplus
extern "C" {
#endif			
	class LogFileAppender:Appender
	{
		private:
			std::string logFilename;
			long maxLogFileSize;
			std::ofstream logfileStream;
			long logfileSize();
			std::string createMsg(SEVERITY sev, std::string Message);

		public:
			LogFileAppender();
			LogFileAppender(SEVERITY minSev);
			LogFileAppender(SEVERITY minSev, std::string logfile, long maxLogfileSize);
			void Log(std::string  Message);
			void Log(SEVERITY sev, std::string Message);
			void SetLogfile(std::string logfile);
			void SetLogfileSize(long logfileSize);
	};
#ifdef __cplusplus
}
#endif