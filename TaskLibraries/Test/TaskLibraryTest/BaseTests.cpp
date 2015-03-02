#include "Tests.h"

TEST(Logging, CreateLogger)
{
	Logger log = LogHelper::GetLogger(DEBUG_LOG_LEVEL, true, false, "", false, 0, "");
}