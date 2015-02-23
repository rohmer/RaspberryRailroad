#include <iostream>
#include "LEDTaskTests.h"
#include "log4cplus/logger.h"
#include "log4cplus/fileappender.h"

using namespace std;

int main(int argc, char *argv[])
{
	log4cplus::initialize();
	SharedAppenderPtr fileAppender(new RollingFileAppender(LOG4CPLUS_TEXT("TaskLibraryTest.log"), 5 * 1024, 5));
	fileAppender->setLayout((std::auto_ptr<Layout>(new TTCCLayout())));
	Logger::getRoot().addAppender(fileAppender);
	Logger log = Logger::getRoot();
	LEDTaskTests ledTest(log);
	ledTest.BlinkRow1Col1();
	return 0;
}