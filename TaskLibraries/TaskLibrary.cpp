#include "TaskLibrary.h"

TaskLibrary::TaskLibrary(Logger logger)
{
	log = logger;	
	log.log(DEBUG_LOG_LEVEL, "TaskLibrary initialized");
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