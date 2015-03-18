#include "DatabaseAppender.h"
using namespace std;
using namespace log4cplus;

DatabaseAppender::DatabaseAppender(string databaseServer, string database, string databaseUser, string databasePassword)
{
	dbServer = databaseServer;
	db = database;
	dbConnection = NULL;
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
	mysql_close(dbConnection);
	destructorImpl();
}

void DatabaseAppender::connectToDB()
{	
	if (dbConnection == NULL)
	{
		dbConnection = mysql_init(NULL);
		if (dbConnection == NULL)
		{
			ostringstream msg;
			msg << "Error creating mysql object: " << mysql_error(dbConnection);
			helpers::LogLog().error(msg.str());
			return;
		}
	}

	if (mysql_real_connect(dbConnection, dbServer.c_str(), dbUser.c_str(), dbPassword.c_str(), db.c_str(), dbPort, NULL, 0)==NULL)
	{
		ostringstream msg;
		msg << "Error connecting to mysql server: " << mysql_error(dbConnection);
		helpers::LogLog().error(msg.str());
		return;
	}	

	if (checkAndCreateTable() == true)
		validConnection = true;
	else
		validConnection = false;
}

bool DatabaseAppender::checkAndCreateTable()
{
	MYSQL_RES *result = mysql_list_tables(dbConnection, "log4cplus");
	bool tableExists = false;
	MYSQL_ROW row;
	int num_fields = mysql_num_fields(result);
	while ((row = mysql_fetch_row(result)))
	{
		if (num_fields > 0)
			if (strcmp(row[0], "log4cplus") == 0)
				tableExists = true;
	}
	mysql_free_result(result);	
	if (tableExists)
		return true;

	// Create the table
	if (mysql_query(dbConnection, "CREATE TABLE log4cplus(timestamp DATETIME, severity TEXT, message TEXT, file TEXT, function TEXT, line INT)"))
	{
		ostringstream msg;
		msg << "Error creating table: " << mysql_error(dbConnection);
		helpers::getLogLog().error(msg.str());
		return false;
	}

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

		helpers::Time logTime = ev.getTimestamp();
		LogLevel severity = ev.getLogLevel();
		string msg = ev.getMessage();
		string file = ev.getFile();
		string function = ev.getFunction();
		int line = ev.getLine();
		string sevString = getLogLevelManager().toString(severity);
		ostringstream query;
		
		query << "INSERT INTO log4cplus VALUES(now(),'" << sevString << "','" << msg << "','" << file << "','" << function << "'," << line << ")";
		if (mysql_query(dbConnection,query.str().c_str()))
		{
			ostringstream msg;
			msg << "Error processing query: " << query << " mysql_err==" << mysql_error(dbConnection);
			helpers::getLogLog().error(msg.str());			
		}

		return;
	}
}