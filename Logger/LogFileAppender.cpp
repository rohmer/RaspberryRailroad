#include "LogFileAppender.h"

void LogFileAppender::SetLogfile(std::string logfile)
{
	if (logfileStream.is_open())
	{
		logfileStream.flush();
		logfileStream.close();
	}
	try
	{
		logfileStream.open(logfile.c_str(), std::ios::ate);
	}
	catch (...)
	{
		return;
	}
}

void LogFileAppender::SetLogfileSize(long logFileSize)
{
	this->maxLogFileSize = logFileSize;
}

LogFileAppender::LogFileAppender():Appender()
{
	SetLogfile("logfile.txt");
	SetLogfileSize(1024 * 1024*5);
}

LogFileAppender::LogFileAppender(SEVERITY sev) :Appender(sev)
{
	SetLogfile("logfile.txt");
	SetLogfileSize(1024 * 1024 * 5);
}

LogFileAppender::LogFileAppender(SEVERITY minSev, std::string logfile, long maxLogfileSize):Appender(minSev)
{
	SetLogfile(logfile);
	SetLogfileSize(maxLogfileSize);
}

long LogFileAppender::logfileSize()
{
	std::ifstream f;
	f.open(logFilename.c_str(), std::ios_base::binary | std::ios_base::in);
	if (!f.good() || f.eof() || !f.is_open()) { return 0; }
	f.seekg(0, std::ios_base::beg);
	std::ifstream::pos_type begin_pos = f.tellg();
	f.seekg(0, std::ios_base::end);
	return static_cast<long>(f.tellg() - begin_pos);
}

std::string LogFileAppender::createMsg(SEVERITY sev, std::string Message)
{
	std::ostringstream retStream;
	retStream << GetTime();
	retStream << " - ";
	switch (sev)
	{
		case INFO:
			retStream << "INFO - ";
			break;
		case WARN:
			retStream << "WARN - ";
			break;
		case ERROR:
			retStream << "ERROR - ";
			break;
	}
	retStream << Message;
	return retStream.str();
}

void LogFileAppender::Log(SEVERITY sev, std::string Message)
{
	if (sev < minLogSeverity)
		return;
	std::string msg = createMsg(sev, Message);
	if (!logfileStream.is_open())
	{
		try
		{
			logfileStream.open(this->logFilename.c_str(), std::ios::ate);
		}
		catch (...)
		{
			return;
		}
	}
	logfileStream << msg;
	if (logfileSize() > maxLogFileSize)
	{
		logfileStream.close();
		logfileStream.open(this->logFilename.c_str(), std::ios::out);
	}
}

void LogFileAppender::Log(std::string Message)
{
	Log(INFO, Message);
}