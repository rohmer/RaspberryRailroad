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

void LEDTaskTests::LightLEDs()
{
	try
	{
		TaskLibrary* tl = new TaskLibrary(DEBUG_LOG_LEVEL, true, false, "", false, 0, "");
		LEDTask* ledPtr = NULL;
		ledPtr = dynamic_cast<LEDTask*> (tl->GetTask(TLEDTask));
		if (!ledPtr)
		{
			TEST_FAIL("Failed to create a LEDTask");
		}
		for (int row = 1; row <= 8; row++)
			for (int col = 1; col <= 8; col++)
			{
				ledPtr->Draw(col, row, true);
				usleep(50000);
				ledPtr->Clear();
			}
	}
	catch (std::exception const & e)
	{
		ostringstream msg;
		msg << "Exception caught in test: " << e.what();
		TEST_FAIL(msg);
	}
}

void LEDTaskTests::ExecuteLEDTask()
{
	try
	{
		TaskLibrary* tl = new TaskLibrary(DEBUG_LOG_LEVEL, true, false, "", false, 0, "");
		LEDTask* ledPtr = NULL;
		ledPtr = dynamic_cast<LEDTask*> (tl->GetTask(TLEDTask));
		if (!ledPtr)
		{
			TEST_FAIL("Failed to create a LEDTask");
		}

		for (int row = 1; row <= 8; row++)
			for (int col = 1; col <= 8; col++)
			{
				vector<int> params;
				params.push_back(0);							// Max to use
				params.push_back(row);
				params.push_back(col);
				params.push_back(1);							// Turn on

				ledPtr->Run(params);
				usleep(50000);
				ledPtr->Clear();
			}
	}
	catch (std::exception const & e)
	{
		ostringstream msg;
		msg << "Exception caught in test: " << e.what();
		TEST_FAIL(msg);
	}
}