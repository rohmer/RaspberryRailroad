#pragma once
// Get the base class
#include "TaskBase/TaskBase.h"
#include "LEDTask/LEDTask.h"
#include "ServoTask/ServoTask.h"

// Now include each type of specialized task
#include "LEDTask/LEDTask.h"
#include "ServoTask/ServoTask.h"

#include <vector>
#include <unistd.h>

enum TaskTypes { TLEDTask, TServoTask };

using namespace std;

class TaskLibrary
{
	private:
		map<TaskTypes,TaskBase*> libsLoaded;
		Logger log;
		bool multiThreaded;

	public:
		TaskLibrary(Logger logger);
		TaskBase *GetTask(TaskTypes);
		void SetMultiThreaded(bool value);
		void RunTask(TaskBase* task, vector<int> args);
};