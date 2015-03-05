#pragma once
#include "Tests.h"

class OccupancyLibraryTests : public Test::Suite
{
public:
	OccupancyLibraryTests()
	{
		Test::TextOutput output(Test::TextOutput::Verbose);
		TEST_ADD(OccupancyLibraryTests::InitializationTest);		
		TEST_ADD(OccupancyLibraryTests::ReadXML1);
	}

	void InitializationTest();	
	void ReadXML1();
};