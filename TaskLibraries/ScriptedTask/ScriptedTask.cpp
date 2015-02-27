#include "ScriptedTask.h"

ScriptedTask::ScriptedTask(Logger logger) : TaskBase(logger)
{
	log.log(DEBUG_LOG_LEVEL,LOG4CPLUS_TEXT("ScriptedTask::ScriptedTask() Initialized"));
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

}
