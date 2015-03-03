#pragma once
#include "Tests.h"
#include "TaskLibrary.h"

class TaskLibraryTestSuite : public Test::Suite
{
	public:
		TaskLibraryTestSuite()
		{
			Test::TextOutput output(Test::TextOutput::Verbose);
			TEST_ADD(TaskLibraryTestSuite::LogStdOutTest);
			TEST_ADD(TaskLibraryTestSuite::LogFileTest);
		}
	private:
		void LogStdOutTest();
		void LogFileTest();
};