#include "TaskLibrary.h"

TaskLibrary::TaskLibrary(Logger logger)
{
	log = logger;	
	multiThreaded = true;
	log.log(DEBUG_LOG_LEVEL, "TaskLibrary initialized (MultiThreaded)");
}

TaskBase *TaskLibrary::GetTask(TaskTypes types)
{
	ostringstream msg;
	msg << "Entering TaskLibrary::GetTask(" << types << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	map<TaskTypes, TaskBase*>::iterator it = libsLoaded.find(types);
	if (it != libsLoaded.end())
	{
		log.log(DEBUG_LOG_LEVEL, "Lib already loaded, returning instance");
		return it->second;
	}
	
	// We haven't created one yet, lets create it
	switch (types)
	{
		case TLEDTask:
		{
			TaskBase * tb = new LEDTask(log);
			libsLoaded.insert(std::pair<TaskTypes, TaskBase*>(TLEDTask, tb));
			log.log(DEBUG_LOG_LEVEL, "Returning new instance of LEDTask");
			return tb;
		}
		case TServoTask:
		{
			TaskBase *tb = new ServoTask(log);
			libsLoaded.insert(std::pair<TaskTypes, TaskBase*>(TServoTask, tb));
			log.log(DEBUG_LOG_LEVEL, "Returning new instance of ServoTask");
			map<TaskTypes, TaskBase*>::iterator it = libsLoaded.find(TServoTask);
			return tb;
		}
	}	
}

void TaskLibrary::SetMultiThreaded(bool value)
{
	ostringstream msg;
	msg << "Entering TaskLibrary::SetMultiThreaded(" << value << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	multiThreaded = value;
	msg.clear();
	msg << "Exiting TaskLibrary::SetMultiThreaded(" << value << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

void TaskLibrary::RunTask(TaskBase * task, vector<int> args)
{
	ostringstream msg;
	msg << "Entering TaskLibrary::RunTask(" << task->GetTaskName() << ",";
	for (int a = 0; a < args.size(); a++)
	{
		msg << ", " << args[a];
	}
	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (!multiThreaded)
	{
		log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Executing task single threaded"));
		task->Run(args);
		log.log(DEBUG_LOG_LEVEL,LOG4CPLUS_TEXT("Exiting TaskLibrary::RunTask()"));
		return;
	}
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Executing task multithreaded"));
	pid_t p = fork();
	if (p == 0)
	{
		task->Run(args);
	}
	else
	{
		if (p < 0)
		{
			log.log(ERROR_LOG_LEVEL, LOG4CPLUS_TEXT("Failed to fork new process, running single threaded"));
			task->Run(args);
		} else
		{
			log.log(WARN_LOG_LEVEL, LOG4CPLUS_TEXT("Task running on parent process"));
			task -> Run(args);
		}
	}
	log.log(DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("Exiting TaskLibrary::RunTask()"));
}
