#pragma once
#include <log4cplus/logger.h>

using namespace log4cplus;

class TaskBase
{
	protected:
		Logger log;

	public:
		TaskBase(Logger logger);
		virtual void Run();
		virtual void Init();
};