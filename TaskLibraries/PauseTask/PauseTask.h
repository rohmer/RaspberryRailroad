#pragma once
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include "../TaskBase/TaskBase.h"
#include <stdlib.h>
#include <unistd.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

// Simplie pauses for a certain amount of milliseconds (Used in scripted tasks)
class PauseTask : public TaskBase
{
private:
	void pause(int milliseconds);
	Logger log;

public:
	PauseTask(Logger logger);
	void Run(vector<int> args);
	void Init();
	std::string GetTaskName() { return "PauseTask"; }
	~PauseTask();
};