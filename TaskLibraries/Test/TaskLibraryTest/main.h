#pragma once
#include "Tests.h"
#include "LEDTaskTests.h"
#include "TaskLibraryTest.h"

class TaskRunner
{
	public:
		int main(int argc, char* argv[]);

	private:
		bool RunTaskLibraryTests();
		bool RunLEDTaskTests();
};