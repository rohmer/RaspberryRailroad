#include "TaskLibrary.h"

TaskLibrary::TaskLibrary(log4cplus::LogLevel minSevToLog, bool logToStdOut, bool logToFile, std::string filename, bool logToNet, int port, std::string hostname)
{
	log = CreateLogger(minSevToLog, logToStdOut, logToFile, filename, logToNet, port, hostname);
	multiThreaded = true;
	log.log(DEBUG_LOG_LEVEL, "TaskLibrary initialized (MultiThreaded)");
}

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
			return tb;
		}
		case TScriptedTask:
			TaskBase *tb = new ScriptedTask(log);
			libsLoaded.insert(std::pair<TaskTypes, TaskBase*>(TScriptedTask, tb));
			log.log(DEBUG_LOG_LEVEL, "Returning new instance of ScriptedTask");
			return tb;
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

Logger TaskLibrary::CreateLogger(log4cplus::LogLevel minSevToLog, bool logToStdOut, bool logToFile, std::string filename, bool logToNet, int port, std::string hostname)
{
	log4cplus::initialize();
	Logger logger = Logger::getRoot();
	log4cplus::getMDC().put(LOG4CPLUS_TEXT("key"),
		LOG4CPLUS_TEXT("MDC value"));
	logger.setLogLevel(minSevToLog);
	log4cplus::tstring pattern = LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S,%Q} [%t] %-5p %c{2} %%%x%% - %X{key} - %m [%l]%n");
	if (logToStdOut)
	{
		SharedObjectPtr<Appender> appender(new ConsoleAppender());

		appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		logger.addAppender(appender);
	}

	if (logToFile)
	{
		SharedFileAppenderPtr appender(
			new RollingFileAppender(LOG4CPLUS_TEXT(filename), 5 * 1024 * 1024, 5,
			false, true));

		appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		appender->getloc();
		logger.addAppender(SharedAppenderPtr(appender.get()));
	}
	if (logToNet)
	{
		tstring serverName = LOG4CPLUS_TEXT(serverName);
		SharedAppenderPtr appender(new SocketAppender(serverName, port));
		appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		logger.addAppender(appender);
	}
	return logger;
}

Logger TaskLibrary::CreateLogger(log4cplus::LogLevel minSevToLog, bool logToStdOut, bool logToFile, std::string filename, bool logToNet, int port, std::string hostname, bool logToDB, std::string databaseServer, std::string database, std::string userName, std::string password, int dbPort)
{
	log4cplus::initialize();
	Logger logger = Logger::getRoot();
	log4cplus::getMDC().put(LOG4CPLUS_TEXT("key"),
		LOG4CPLUS_TEXT("MDC value"));
	logger.setLogLevel(minSevToLog);
	log4cplus::tstring pattern = LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S,%Q} [%t] %-5p %c{2} %%%x%% - %X{key} - %m [%l]%n");
	if (logToStdOut)
	{
		SharedObjectPtr<Appender> appender(new ConsoleAppender());

		appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		logger.addAppender(appender);
	}

	if (logToFile)
	{
		SharedFileAppenderPtr appender(
			new RollingFileAppender(LOG4CPLUS_TEXT(filename), 5 * 1024 * 1024, 5,
			false, true));

		appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		appender->getloc();
		logger.addAppender(SharedAppenderPtr(appender.get()));
	}
	if (logToNet)
	{
		tstring serverName = LOG4CPLUS_TEXT(serverName);
		SharedAppenderPtr appender(new SocketAppender(serverName, port));
		appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		logger.addAppender(appender);
	}
	if (logToDB)
	{
		tstring dbServer = LOG4CPLUS_TEXT(dbServer);
		tstring db = LOG4CPLUS_TEXT(db);
		tstring dbUser = LOG4CPLUS_TEXT(userName);
		tstring dbPassword = LOG4CPLUS_TEXT(password);
		SharedAppenderPtr appender(new DatabaseAppender(dbServer, database, dbUser, dbPassword, dbPort));
		log4cplus::tstring dbPattern = LOG4CPLUS_TEXT("%m");
		appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(dbPattern)));
		logger.addAppender(appender);
	}
	return logger;
}
