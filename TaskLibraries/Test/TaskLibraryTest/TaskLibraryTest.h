#pragma once
#include "Tests.h"
#include "TaskLibrary.h"
#include "DatabaseUtilities.h"

class TaskLibraryTestSuite : public Test::Suite
{
	public:
		TaskLibraryTestSuite()
		{
			Test::TextOutput output(Test::TextOutput::Verbose);
			TEST_ADD(TaskLibraryTestSuite::LogStdOutTest);
			TEST_ADD(TaskLibraryTestSuite::LogFileTest);
			TEST_ADD(TaskLibraryTestSuite::NetworkLogTest);
			TEST_ADD(TaskLibraryTestSuite::DatabaseLogTest);
		}
	
		void LogStdOutTest();
		void LogFileTest();
		void NetworkLogTest();
		void DatabaseLogTest();
};