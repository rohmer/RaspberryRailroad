#pragma once
#include <log4cplus/logger.h>
#include <vector>

using namespace log4cplus;
using namespace std;

class TaskBase
{
	protected:
		Logger log;

	public:
		TaskBase(Logger logger);
		virtual void Run(vector<int> args);
		virtual void Init();
};