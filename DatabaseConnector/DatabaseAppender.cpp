#include "DatabaseAppender.h"
using namespace std;
using namespace log4cplus;

DatabaseAppender::DatabaseAppender(string databaseServer, string database, string databaseUser, string databasePassword)
{
	dbServer = databaseServer;
	db = database;
	dbConnection = NULL;
	dbDriver = NULL;
	dbUser = databaseUser;
	dbPassword = databasePassword;	
	validConnection = false;
}

DatabaseAppender::DatabaseAppender(helpers::Properties const & properties)
	: Appender(properties)
{
	validConnection = false;
	properties.getString(dbServer, LOG4CPLUS_TEXT("databaseServer"));
	properties.getString(db, LOG4CPLUS_TEXT("database"));
	properties.getString(dbUser, LOG4CPLUS_TEXT("databaseUser"));
	properties.getString(dbPassword, LOG4CPLUS_TEXT("databasePassword"));
	properties.getInt(dbPort, LOG4CPLUS_TEXT("dbPort"));
	dbConnection = NULL;
}

DatabaseAppender::~DatabaseAppender()
{
	dbConnection->close();
	destructorImpl();
}

void DatabaseAppender::connectToDB()
{	
	if (dbDriver == NULL)
	{
		dbDriver = get_driver_instance();
	}
	if (dbConnection == NULL)
	{
		try
		{
			dbConnection = dbDriver->connect(dbServer.c_str(), dbUser.c_str(), dbPassword.c_str());
			dbConnection->setSchema(db.c_str());

			if (dbConnection == NULL)
			{
				helpers::LogLog().error("Failed to create database connection");
				return;
			}
		}
		catch (sql::SQLException &e)
		{
			ostringstream msg;
			msg << "ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
			helpers::LogLog().error(msg.str());
			return;
		}
	}

	if (checkAndCreateTable() == true)
		validConnection = true;
	else
		validConnection = false;
}

bool DatabaseAppender::checkAndCreateTable()
{
	sql::Statement *stmt = dbConnection->createStatement();
	sql::ResultSet *res = stmt->executeQuery("show tables like 'log4cplus'");
	if (res->rowsCount() == 1)
		return true;
	
	try
	{
		// Create the table
		if (!stmt->execute("CREATE TABLE log4cplus(timestamp DATETIME, severity TEXT, message TEXT, file TEXT, function TEXT, line INT)"))
		{
			ostringstream msg;
			msg << "Error creating table: ";
			helpers::getLogLog().error(msg.str());
			stmt->close();
			return false;
		}
	}
	catch (sql::SQLException &e)
	{
		ostringstream msg;
		msg << "ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
		helpers::LogLog().error(msg.str());
		stmt->close();
		return false;
	}
	stmt->close();
	return true;
}

void DatabaseAppender::append(spi::InternalLoggingEvent const & ev)
{
	if (!validConnection)
	{
		connectToDB();
		if (!validConnection)
		{
			helpers::getLogLog().error("Cannot create a valid connection, log message will be lost");
			return;
		}
	}
	if (dbConnection->isClosed())
		dbConnection->reconnect();
	helpers::Time logTime = ev.getTimestamp();
	LogLevel severity = ev.getLogLevel();
	string msg = ev.getMessage();
	string file = ev.getFile();
	string function = ev.getFunction();
	int line = ev.getLine();
	string sevString = getLogLevelManager().toString(severity);
	ostringstream query;

	try
	{
		sql::Statement *stmt = dbConnection->createStatement();
		query << "INSERT INTO log4cplus VALUES(now(),'" << sevString << "','" << msg << "','" << file << "','" << function << "'," << line << ")";
		if (!stmt->execute(query.str().c_str()))
		{
			ostringstream msg;
			msg << "Error processing query: " << query;
			helpers::getLogLog().error(msg.str());
		}
	}
	catch (sql::SQLException &e)
	{
		ostringstream msg;
		msg << "ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
		helpers::LogLog().error(msg.str());
		return;
	}

	return;
}