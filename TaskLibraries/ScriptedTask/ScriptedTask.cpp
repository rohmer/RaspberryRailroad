#include "ScriptedTask.h"

ScriptedTask::ScriptedTask(Logger logger) : TaskBase(logger)
{
	log.log(DEBUG_LOG_LEVEL,LOG4CPLUS_TEXT("ScriptedTask::ScriptedTask() Initialized"));
}

int ScriptedTask::CreateTaskChain(std::string TaskChainName, int TaskGroup)
{
	ostringstream msg;
	msg << "Entering ScriptedTask::CreateTaskChain(" << TaskChainName << ", "<<TaskGroup<<")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	TaskChain taskChain;
	taskChain.Name = TaskChainName;
	taskChain.TaskGroup = TaskGroup;
	taskChains.insert(std::pair<int, TaskChain>(taskIDCounter, taskChain));
	msg << ", rc==" << taskIDCounter;
	taskIDCounter++;
	return (taskIDCounter);
}

/// <summary>
/// Creates a new task chain.  This is used to start building a task chain
/// </summary>
/// <param name="TaskChainName">Name of the task chain.</param>
/// <returns>int - This is the ID used to cancel a task chain</returns>
int ScriptedTask::CreateTaskChain(std::string TaskChainName)
{
	ostringstream msg;
	msg << "Entering ScriptedTask::CreateTaskChain(" << TaskChainName << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	TaskChain taskChain;
	taskChain.Name = TaskChainName;
	taskChain.TaskGroup = -1;
	taskChains.insert(std::pair<int,TaskChain>(taskIDCounter, taskChain));
	msg << ", rc==" << taskIDCounter;
	taskIDCounter++;	
	return (taskIDCounter);
}

bool ScriptedTask::AddTaskToTaskChain(int taskChainID, TaskItem taskItem)
{
	log.log(DEBUG_LOG_LEVEL, "Entering ScriptedTask::AddTaskToTaskChain()");
	if (taskChains.find(taskChainID)==taskChains.end())
	{
		// Key doesnt exist
		ostringstream msg;
		msg << "Key: " << taskChainID << " does not exist.  Did you forget to call CreateTaskChain?";
		log.log(ERROR_LOG_LEVEL, msg.str());
		log.log(DEBUG_LOG_LEVEL, "Exiting ScriptedTask::AddTaskToTaskChain(), rc==false");
		return false;
	}
	// Add it to the chain
	taskChains.find(taskChainID)->second.Tasks.push_back(taskItem);
	log.log(DEBUG_LOG_LEVEL, "Exiting ScriptedTask::AddTaskToTaskChain(), rc==true");
	return true;
}

void ScriptedTask::Run(vector<int> args)
{
	if (args.size() == 0)
	{
		log.log(ERROR_LOG_LEVEL, "ScriptedTask::Run must be run with 1 argument");
		return;
	}
	ostringstream msg;
	msg << "Entering ScriptedTask::Run(";
	int taskChainID = args[0];
	msg << taskChainID << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	
	pid_t pid = fork();
	if (pid == 0)
	{
		// successful fork
		runTask(taskChainID, true);				// run multithreaded, as planned
	}
	else if(pid<0)
	{
		log.log(WARN_LOG_LEVEL, "Failed to fork process, this is running on the run thread");
		runTask(taskChainID, false);			// run single threaded, i.e. dont repeat and block thread 
	}
	else
	{
		msg.clear();
		msg << "Exiting ScriptedTask::Run(";
		int taskChainID = args[0];
		msg << taskChainID << ")";
		log.log(DEBUG_LOG_LEVEL, msg.str());
		return;
	}
}

/// <summary>
/// Runs the task (Hopefully on a thread), sequentially executing each of the tasks
/// </summary>
/// <param name="taskChainID">The task chain identifier.</param>
void ScriptedTask::runTask(int taskChainID, bool threadMode)
{
	ostringstream msg;
	msg << "Entering ScriptedTask::runTask(" << taskChainID << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (taskChains.find(taskChainID) == taskChains.end())
	{
		// Key doesnt exist
		ostringstream msg;
		msg << "Key: " << taskChainID << " does not exist.  Did you forget to call CreateTaskChain?";
		log.log(ERROR_LOG_LEVEL, msg.str());
		log.log(DEBUG_LOG_LEVEL, "Exiting ScriptedTask::runTask()");
		return;
	}
	TaskChain tc = taskChains.find(taskChainID)->second;
	bool repeat = true;
	while (repeat)
	{
		TaskBase *task;
		for (int a = 0; a < tc.Tasks.size(); a++)
		{
			// Check to see if this guy is to be canceled
			if (tasksToCancel[a] == taskChainID)
			{
				repeat = false;
				msg.clear();
				msg << "This task is scheduled to be canceled, taskID: " << taskChainID;

				// Remove it from vector
				tasksToCancel.erase(remove(tasksToCancel.begin(), tasksToCancel.end(), taskChainID), tasksToCancel.end());
			}
			TaskItem taskItem = tc.Tasks[a];
			task = getTask(taskItem.TaskType);
			usleep(taskItem.IntervalBeforeExecution * 1000);
			task->Run(taskItem.TaskArguments);			
		}
		repeat = threadMode;
		if (tc.RepeatInterval == 0)
			repeat = false;
		else
			usleep(tc.RepeatInterval * 1000);
		// Check to see if this guy is to be canceled
		for (int a = 0; a < tasksToCancel.size(); a++)
		{
			if (tasksToCancel[a] == taskChainID)
			{
				repeat = false;
				msg.clear();
				msg << "This task is scheduled to be canceled, taskID: " << taskChainID;

				// Remove it from vector
				tasksToCancel.erase(remove(tasksToCancel.begin(), tasksToCancel.end(), taskChainID), tasksToCancel.end());
			}
		}
	}
	msg.clear();
	msg << "Entering ScriptedTask::runTask(" << taskChainID << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());

}

TaskBase* ScriptedTask::getTask(TaskTypes ttype)
{
	ostringstream msg;
	msg << "Entering TaskLibrary::GetTask(" << ttype << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	map<TaskTypes, TaskBase*>::iterator it = loadedTasks.find(ttype);
	if (it != loadedTasks.end())
	{
		log.log(DEBUG_LOG_LEVEL, "Lib already loaded, returning instance");
		return it->second;
	} 

	// We haven't created one yet, lets create it
	switch (ttype)
	{
	case TLEDTask:
	{
		TaskBase * tb = new LEDTask(log);
		loadedTasks.insert(std::pair<TaskTypes, TaskBase*>(TLEDTask, tb));
		log.log(DEBUG_LOG_LEVEL, "Returning new instance of LEDTask");
		tb->Init();
		return tb;
	}
	case TServoTask:
	{
		TaskBase *tb = new ServoTask(log);
		loadedTasks.insert(std::pair<TaskTypes, TaskBase*>(TServoTask, tb));
		log.log(DEBUG_LOG_LEVEL, "Returning new instance of ServoTask");
		tb->Init();
		return tb;
	}
	case TScriptedTask:
		TaskBase *tb = new ScriptedTask(log);
		loadedTasks.insert(std::pair<TaskTypes, TaskBase*>(TScriptedTask, tb));
		log.log(DEBUG_LOG_LEVEL, "Returning new instance of ScriptedTask");
		tb->Init();
		return tb;
	}
}

void ScriptedTask::CancelTask(int taskID)
{
	ostringstream msg;
	msg << "Entering ScriptedTask::CancelTask(" << taskID << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	tasksToCancel.push_back(taskID);
	msg.clear();
	msg << "Exiting ScriptedTask::CancelTask(" << taskID << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void ScriptedTask::cancelTasksInGroup(int GroupNumber)
{
	ostringstream msg;
	msg << "Entering ScriptedTask::cancelTasksInGroup(" << GroupNumber << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());

	if (GroupNumber == -1)
	{
		msg.clear();
		msg << "Exiting ScriptedTask::cancelTasksInGroup(" << GroupNumber << ")";
		log.log(DEBUG_LOG_LEVEL, msg.str());
	}
	map<int, TaskChain>::iterator it;
	for (it = taskChains.begin(); it != taskChains.end(); it++)
	{
		if (it->second.TaskGroup == GroupNumber)
		{
			CancelTask(it->first);
		}
	}
	msg.clear();
	msg << "Exiting ScriptedTask::cancelTasksInGroup(" << GroupNumber << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}