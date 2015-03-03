#include "LEDTaskTests.h"

void LEDTaskTests::CreateLEDTask()
{
	try
	{
		TaskLibrary* tl = new TaskLibrary(DEBUG_LOG_LEVEL, true, false, "", false, 0, "");
		LEDTask* ledPtr = NULL;
		ledPtr=dynamic_cast<LEDTask*> (tl->GetTask(TLEDTask));
		if (!ledPtr)
		{
			TEST_FAIL("Failed to create a LEDTask");
		}

		if (ledPtr->GetTaskName().compare("LEDTask") != 0)
		{
			TEST_FAIL("GetTaskName() did not return \"LEDTask\" for an LEDTask");
		}
	}
	catch (std::exception const & e)
	{
		ostringstream msg;
		msg << "Exception caught in test: " << e.what();
		TEST_FAIL(msg);
	}
}
