#include "OccupancyLibraryTests.h"

void OccupancyLibraryTests::InitializationTest()
{
	OccupancyLibrary *ol = new OccupancyLibrary(WARN_LOG_LEVEL, true, false, "", false, 0, "");

}

void OccupancyLibraryTests::ReadXML1()
{
	OccupancyLibrary *ol = new OccupancyLibrary(DEBUG_LOG_LEVEL, true, false, "", false, 0, "");
	bool result=ol->ImportXML("TestBlocks.xml");
	if (!result)
		TEST_FAIL("Failed to read TestBlocks.xml");
	
	// Check to see our blocks look right
	for (int a = 1; a <= 5; a++)
	{
		ostringstream blockName;
		blockName << "Block" << a;
		Block* block = ol->GetBlock(blockName.str());
		if (block == NULL)
		{
			ostringstream eMsg;
			eMsg << "Failed to get expected block " << blockName;		
			ol->GetLogger().log(ERROR_LOG_LEVEL, eMsg.str());
			TEST_FAIL("Failed to get expected block");
		}
		else
		{			
			// Now check the name is right
			if (blockName.str().compare(block->GetBlockName()) != 0)
			{
				ostringstream eMsg;
				eMsg << "Expected block name: " << blockName.str().c_str() << " received: " << block->GetBlockName();
				ol->GetLogger().log(ERROR_LOG_LEVEL, eMsg.str());
				TEST_FAIL("Failed to get expected block name");
			}
			else
			{
				for (int b = 0; b < 3; b++)
				{
					int sensorID = ((a - 1) * 3) + b + 1;					
					bool found = false;
					ostringstream msg;
					msg << "Checking sensorID: " << sensorID;
					ol->GetLogger().log(DEBUG_LOG_LEVEL, msg.str());
					for (int c = 0; c < 3 && c<block->GetDetectors().size(); c++)
					{
						msg.clear();
						msg << "Block sensor #" << c << " == " << block->GetDetectors()[c];
						ol->GetLogger().log(DEBUG_LOG_LEVEL, msg.str());

						if (block->GetDetectors()[c] == sensorID)
						{
							found = true;
						}
					}
					if (!found)
					{
						ostringstream eMsg;
						eMsg << "Expected to find SensorID: " << sensorID << " and did not for block: " << blockName.str();
						eMsg << "\nBlock had the following Sensors: ";
						for (int d = 0; d < block->GetDetectors().size(); d++)
							eMsg << block->GetDetectors()[d] << ", ";

						ol->GetLogger().log(ERROR_LOG_LEVEL, eMsg.str());
						TEST_FAIL("Failed to get SensorID");
					}
				}
			}
			// Now check connections
			int blockID = block->GetID();
			// Connections are -1 and +1
			int lowConnection = blockID - 1;
			int highConnection = blockID + 1;
			if (lowConnection < 0)
				lowConnection = 4;
			if (highConnection > 4)
				highConnection = 0;
			if (block->GetNeighbors().size() != 2)
			{
				ostringstream eMsg;
				eMsg << "Exptected to find 2 neighbors, found: " << block->GetNeighbors().size();
				ol->GetLogger().log(ERROR_LOG_LEVEL, eMsg.str());
				TEST_FAIL("Failed to get neighbors for block");
			}
			if (block->GetNeighbors()[0] != lowConnection && block->GetNeighbors()[0] != highConnection)
			{
				ostringstream eMsg;
				eMsg << "Expected to find either: " << lowConnection << " or " << highConnection << " in connection[0], found: " << block->GetNeighbors()[0];
				ol->GetLogger().log(ERROR_LOG_LEVEL, eMsg.str());
				TEST_FAIL("Failed to get neighbors for block");
			}
			if (block->GetNeighbors()[1] != lowConnection && block->GetNeighbors()[1] != highConnection)
			{
				ostringstream eMsg;
				eMsg << "Expected to find either: " << lowConnection << " or " << highConnection << " in connection[1], found: " << block->GetNeighbors()[1];
				ol->GetLogger().log(ERROR_LOG_LEVEL, eMsg.str());
				TEST_FAIL("Failed to get neighbors for block");
			}
		}
	}
}