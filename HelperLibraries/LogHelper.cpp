#include "LogHelper.h"

Logger LogHelper::GetLogger(log4cplus::LogLevel minSevToLog, bool logToStdOut, 
							bool logToFile, std::string logFile, 
							bool logToNet, int port, std::string hostname)
{
	log4cplus::initialize();
	Logger logger = Logger::getRoot();	
	log4cplus::getMDC().put(LOG4CPLUS_TEXT("key"),
		LOG4CPLUS_TEXT("MDC value"));
	logger.setLogLevel(minSevToLog);
	log4cplus::tstring pattern= LOG4CPLUS_TEXT("%d{%m/%d/%y %H:%M:%S,%Q} [%t] %-5p %c{2} %%%x%% - %X{key} - %m [%l]%n");
	if (logToStdOut)
	{
		SharedObjectPtr<Appender> appender(new ConsoleAppender());
		
		appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		logger.addAppender(appender);
	}

	if (logToFile)
	{
		SharedFileAppenderPtr appender(
			new RollingFileAppender(LOG4CPLUS_TEXT(logFile), 5 * 1024*1024, 5,
			false, true));

		appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		appender->getloc();
		logger.addAppender(SharedAppenderPtr(appender.get()));
	}
	if (logToNet)
	{
		tstring serverName = LOG4CPLUS_TEXT(serverName);
		SharedAppenderPtr appender(new SocketAppender(serverName, port));
		appender->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		logger.addAppender(appender);
	}
	return logger;
}