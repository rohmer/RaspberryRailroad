// Get the base class
#include "TaskBase/TaskBase.h"
#include "LEDTask/LEDTask.h"
#include "ServoTask/ServoTask.h"
// Now include each type of specialized task
#include "LEDTask/LEDTask.h"
#include "ServoTask/ServoTask.h"
#include "PauseTask\PauseTask.h"
#include <log4cplus\consoleappender.h>
#include <log4cplus\fileappender.h>
#include <log4cplus\socketappender.h>
#include <log4cplus/ndc.h>
#include <log4cplus/mdc.h>
#include <log4cplus/layout.h>
#include <vector>
#include <unistd.h>
#include "ScriptedTask/ScriptedTask.h"
#include "../DatabaseConnector/DatabaseAppender.h"

#ifndef TaskLibrarySet
#define TaskLibrarySet
enum TaskTypes { TLEDTask, TServoTask, TScriptedTask, TPauseTask };

using namespace std;

class TaskLibrary
{
	private:
		map<TaskTypes,TaskBase*> libsLoaded;
		Logger log;
		bool multiThreaded;

	public:
		TaskLibrary(log4cplus::LogLevel minSevToLog, bool logToStdOut, bool logToFile, std::string filename, bool logToNet, int port, std::string hostname);
		TaskLibrary(log4cplus::LogLevel minSevToLog, bool logToStdOut, bool logToFile, std::string filename, bool logToNet, int port, std::string hostname, bool logToDB, std::string databaseServer, std::string database, std::string userName, std::string password, int dbPort);
		TaskLibrary(Logger logger);
		TaskBase *GetTask(TaskTypes);
		void SetMultiThreaded(bool value);
		void RunTask(TaskBase* task, vector<int> args);	
		static Logger CreateLogger(log4cplus::LogLevel minSevToLog, bool logToStdOut, bool logToFile, std::string filename, bool logToNet, int port, std::string hostname);
		static Logger CreateLogger(log4cplus::LogLevel minSevToLog, bool logToStdOut, bool logToFile, std::string filename, bool logToNet, int port, std::string hostname, bool logToDB, std::string databaseServer, std::string database, std::string userName, std::string password, int dbPort);
		Logger GetLogger() { return log; }
};
#endif