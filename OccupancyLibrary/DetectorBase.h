#pragma once
#include "log4cplus\logger.h"

using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace std;

class DetectorBase
{
private:
	Logger log;

public:
	DetectorBase(Logger logger);
	virtual bool IsTriggered(int id)=0;
	virtual bool IsTriggered() = 0;
};