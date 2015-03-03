#include "TaskLibraryTest.h"

// Unit tests for the TaskLibrary

/// <summary>
/// Check to see if we can create a logger to stdout
/// </summary>
void TaskLibraryTestSuite::LogStdOutTest()
{
	try
	{
		Logger log = TaskLibrary::CreateLogger(DEBUG_LOG_LEVEL, true, false, "", false, 0, "");
		ostringstream strCout;
		cout.rdbuf(strCout.rdbuf());
		log.log(DEBUG_LOG_LEVEL, "DEBUG MESSAGE");
		size_t found = strCout.str().find("DEBUG MESSAGE");
		if (found == std::string::npos)
			TEST_FAIL("Failed to find debug message on console");
		strCout.clear();
		log.log(WARN_LOG_LEVEL, "WARN MESSAGE");
		found = strCout.str().find("WARN MESSAGE");
		if (found == std::string::npos)
			TEST_FAIL("Failed to find warning message on console");
		strCout.clear();
		log.log(ERROR_LOG_LEVEL, "ERROR MESSAGE");
		found = strCout.str().find("ERROR MESSAGE");
		if (found == std::string::npos)
			TEST_FAIL("Failed to find ERROR message on console");
		strCout.clear();
		log.log(FATAL_LOG_LEVEL, "FATAL MESSAGE");
		found = strCout.str().find("FATAL MESSAGE");
		if (found == std::string::npos)
			TEST_FAIL("Failed to find FATAL message on console");		
	}
	catch (std::exception const & e)
	{
		ostringstream msg;
		msg << "Exception caught in test: " << e.what();
		TEST_FAIL(msg);
	}
}

void TaskLibraryTestSuite::LogFileTest()
{

}