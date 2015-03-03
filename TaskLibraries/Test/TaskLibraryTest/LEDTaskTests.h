#pragma once
#include "Tests.h"

class LEDTaskTests : public Test::Suite
{
	public:
		LEDTaskTests()
		{
			Test::TextOutput output(Test::TextOutput::Verbose);
			TEST_ADD(LEDTaskTests::CreateLEDTask);
		}

	
		void CreateLEDTask();

};