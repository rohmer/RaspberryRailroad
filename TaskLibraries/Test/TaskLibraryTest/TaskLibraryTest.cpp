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
		streambuf *oldBuf = cout.rdbuf();
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
		cout.rdbuf(oldBuf);
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
	try
	{
		Logger log = TaskLibrary::CreateLogger(DEBUG_LOG_LEVEL, false, true, "LogFileTest.txt", false, 0, "");
		log.log(DEBUG_LOG_LEVEL, "DEBUG MESSAGE");
		log.log(WARN_LOG_LEVEL, "WARN MESSAGE");
		log.log(ERROR_LOG_LEVEL, "ERROR MESSAGE");
		log.log(FATAL_LOG_LEVEL, "FATAL MESSAGE");
		ifstream infile("LogFileTest.txt", ios::in);
		if (!infile.is_open())
		{
			ostringstream msg;
			msg << "Failed to open LogFileTest.txt";
			TEST_FAIL(msg);
		}
		std::string line;
		int counter = 0;
		size_t found;
		while (std::getline(infile, line))
		{
			switch (counter)
			{
				case 0:
				{
					found = line.find("DEBUG MESSAGE");
					if (found == std::string::npos)
						TEST_FAIL("Failed to find debug message in log file");
					break;
				}
				case 1:
				{
					found = line.find("WARN MESSAGE");
					if (found == std::string::npos)
						TEST_FAIL("Failed to find warn message in log file");
					break;
				}
				case 2:
				{
					found = line.find("ERROR MESSAGE");
					if (found == std::string::npos)
						TEST_FAIL("Failed to find error message in log file");
					break;
				}
				case 4:
				{
					found = line.find("FATAL MESSAGE");
					if (found == std::string::npos)
						TEST_FAIL("Failed to find fatal message in log file");
					break;
				}
			}
			counter++;
		}
		infile.close();
	}
	catch (std::exception const & e)
	{
		ostringstream msg;
		msg << "Exception caught in test: " << e.what();
		TEST_FAIL(msg);
	}
}

void TaskLibraryTestSuite::NetworkLogTest()
{
	//no-op now
	TEST_ASSERT(true);
}
