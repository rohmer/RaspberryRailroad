#include "Appender.h"

Appender::Appender()
{	
	this->minLogSeverity = WARN;
}

Appender::Appender(SEVERITY minSev)
{
	this->minLogSeverity = minSev;
}

void Appender::SetSeverity(SEVERITY sev)
{
	this->minLogSeverity = sev;
}

std::string Appender::GetTime()
{
	time_t timer;
	time(&timer);
	tm *pTime = gmtime(&timer);
	std::ostringstream retStream;	
	retStream << pTime->tm_mon << "/" << pTime->tm_mday << "/" << pTime->tm_year << " " << pTime->tm_hour << ":" << pTime->tm_min << ":" << pTime->tm_sec;
	return retStream.str();
}