#include "OccupancyLibraryTests.h"

void OccupancyLibraryTests::InitializationTest()
{
	OccupancyLibrary *ol = new OccupancyLibrary(WARN_LOG_LEVEL, true, false, "", false, 0, "");

}

void OccupancyLibraryTests::ReadXML1()
{
	OccupancyLibrary *ol = new OccupancyLibrary(WARN_LOG_LEVEL, true, false, "", false, 0, "");
	bool result=ol->ImportXML("TestBlocks.xml");
	if (!result)
		TEST_FAIL("Failed to read TestBlocks.xml");
}