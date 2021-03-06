#pragma once

// A scripted task runs in one of 3 modes
// 1. Runs a list of tasks with a delay between them once
// 2. Runs a list of tasks with a delay between them over and over
// 3. Runs in mode 2, but with the ability to cancel

// NOTE: This should be run in multithreaded mode.  If not, tasks run one at a time

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include "../TaskBase/TaskBase.h"
#include "../LEDTask/LEDTask.h"
#include "../ServoTask/ServoTask.h"
#include <signal.h>
#include <stdlib.h>
#include "../TaskLibrary.h"

struct TaskItem
{
	long IntervalBeforeExecution;
	TaskTypes TaskType;
	vector<int> TaskArguments;
};

struct TaskChain
{
	long RepeatInterval;		// 0 = No Repeat
	vector<TaskItem> Tasks;	
	std::string Name;			// Nice string name
	int TaskGroup;				// Only one task can be running in a specific group
								// So use this for things like occupancy detection, where one chain is running when
								// occupied and one when not occupied, both in the same group
};

// A ScriptedTask consists of:
//		A chain of subtasks (TaskItem), with a timer between them (TaskChain)
//		In TaskChain is the repeat
class ScriptedTask : public TaskBase
{
	private:
		int taskIDCounter;
		map<int, TaskChain> taskChains;			// Int is the taskID, TaskChain is the actual taskChain
		void runTask(int taskChainID, bool threadMode);
		map<TaskTypes,TaskBase*> loadedTasks;
		TaskBase *getTask(TaskTypes);
		vector<int> tasksToCancel;
		void cancelTasksInGroup(int GroupNumber);

	public:
		ScriptedTask(Logger logger);
		std::string GetTaskName() { return "ScriptedTask";  }
		int CreateTaskChain(std::string TaskChainName);
		int CreateTaskChain(std::string, int TaskGroup);
		bool AddTaskToTaskChain(int taskChainID, TaskItem);
		void Run(vector<int> args);
		void CancelTask(int taskID);			
		void Init() {}							// Not used
};