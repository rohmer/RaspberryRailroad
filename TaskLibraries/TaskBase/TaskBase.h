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
		virtual void Run(vector<int> args)=0;
		virtual void Init()=0;
		virtual std::string GetTaskName() = 0;
		
};