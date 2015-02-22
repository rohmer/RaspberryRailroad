#pragma once
// Get the base class
#include "TaskBase/TaskBase.h"

// Now include each type of specialized task
#include "LEDTask/LEDTask.h"
#include "ServoTask/ServoTask.h"

#include <vector>

enum TaskTypes { TLEDTask, TServoTask };

class TaskLibrary
{
	private:
		map<TaskTypes,TaskBase*> libsLoaded;
		Logger log;

	public:
		TaskLibrary(Logger logger);
		TaskBase *GetTask(TaskTypes);
};