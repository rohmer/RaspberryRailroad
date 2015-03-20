#include "DatabaseUtilities.h"

sql::Connection *DatabaseUtilities::getDBConnection(string dbUser, string dbPass, string database, string dbServer)
{
	sql::Driver *dbDriver = get_driver_instance();
	sql::Connection *dbConnection;
	try
	{
		dbConnection = dbDriver->connect(dbServer.c_str(), dbUser.c_str(), dbPass.c_str());
		dbConnection->setSchema(database.c_str());

		if (dbConnection == NULL)
		{
			return NULL;
		}		
	}
	catch (sql::SQLException &e)
	{
		return NULL;
	}

	return dbConnection;
}

bool DatabaseUtilities::TableExists(string dbUser, string dbPass, string database, string dbServer, string table)
{
	try
	{
		sql::Connection *dbConnection = getDBConnection(dbUser, dbPass, database, dbServer);
		if (dbConnection == NULL)			
			return false;

		sql::Statement *stmt = dbConnection->createStatement();
		sql::ResultSet *res = stmt->executeQuery("show tables like 'logs'");
		if (res->rowsCount() == 1)
			return true;
	
		stmt->close();
		return false;
	} 
	catch (sql::SQLException &e)
	{
		cout << e.what();
		return false;
	}
	
}

bool DatabaseUtilities::ClearTable(string dbUser, string dbPass, string database, string dbServer, string table)
{
	if (!TableExists(dbUser, dbPass, database, dbServer, table))
		return true;
	try
	{
		sql::Connection *dbConnection = getDBConnection(dbUser, dbPass, database, dbServer);
		if (dbConnection == NULL)			
			return false;

		sql::Statement *stmt = dbConnection->createStatement();
		ostringstream statement;
		statement << "TRUNCATE TABLE " << table;
		if (!stmt->execute(statement.str()))
		{
			dbConnection->close();
			return false;
		}
	} 
	catch (sql::SQLException &e)
	{
		cout << e.what();
		return false;
	}
	return true;
}