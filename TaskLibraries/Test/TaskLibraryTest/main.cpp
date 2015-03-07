#include "Tests.h"
#include "LEDTaskTests.h"
#include "TaskLibraryTest.h"
#include "OccupancyLibraryTests.h"

// Eventually will give options to run suites, now just gonna run em all

int main(int argc, char* argv[])
{
	Test::TextOutput output(Test::TextOutput::Verbose, std::cout);
	//TaskLibraryTestSuite tlts;
	//tlts.run(output, false);
	//LEDTaskTests ltt;
	//ltt.run(output, false);
	OccupancyLibraryTests olt;
	olt.run(output, false);
}