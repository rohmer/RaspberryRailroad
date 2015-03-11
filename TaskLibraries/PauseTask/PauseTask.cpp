#include "PauseTask.h"

PauseTask::PauseTask(Logger logger) : TaskBase(logger)
{	
	log.log(DEBUG_LOG_LEVEL,"PauseTask::PauseTask() initalized");
}

/// <summary>
/// NoOp
/// </summary>
void PauseTask::Init()
{
}

/// <summary>
/// Pauses for the number of milliseconds in args[0]
/// </summary>
/// <param name="args">The arguments.</param>
void PauseTask::Run(vector<int> args)
{
	ostringstream msg;
	ostringstream argMsg;
	for (int a = 0; a < args.size(); a++)
	{
		argMsg << args[a] << ",";
	}
	msg << "Entering PauseTask::Run(" << argMsg << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	if (args.size() < 1)
	{
		log.log(ERROR_LOG_LEVEL, "Error, args must have 1 item in it for PauseTask::Run, the number of milliseconds to pause");
		msg.clear();
		msg << "Exiting PauseTask::Run(" << argMsg << ")";
		log.log(DEBUG_LOG_LEVEL, msg.str());
		return;
	}
	if (args.size() > 1)
	{		
		msg.clear();
		msg << "PauseTask::Run only uses the first argument, " << args.size() << " were sent.  Ignoring additional arguments";
		log.log(WARN_LOG_LEVEL, msg.str());		
	}

	int pauseSec = args[0];
	msg.clear();
	msg << "Pausing: " << pauseSec << " milliseconds";
	log.log(DEBUG_LOG_LEVEL, msg.str());
	usleep(pauseSec * 1000);
	msg.clear();
	msg << "Exiting PauseTask::Run(" << argMsg << ")";
	log.log(DEBUG_LOG_LEVEL, msg.str());
}

/// <summary>
/// Finalizes an instance of the <see cref="PauseTask"/> class.
/// NoOp
/// </summary>
PauseTask::~PauseTask()
{
}