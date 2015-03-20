#pragma once
#include <string>
#include <sstream>
#include <string.h>
#include <time.h>
#include "../3rdParty/mysqlconnector/mysql_connection.h"
#include "../3rdParty/mysqlconnector/cppconn/driver.h"
#include "../3rdParty/mysqlconnector/cppconn/exception.h"
#include "../3rdParty/mysqlconnector/cppconn/resultset.h"
#include "../3rdParty/mysqlconnector/cppconn/statement.h"

using namespace std;

class DatabaseUtilities
{
	private:
		static sql::Connection *getDBConnection(string dbUser, string dbPass, string database, string dbServer);		

	public:
		static bool ClearTable(string dbUser, string dbPass, string database, string dbServer, string table);
		static bool TableExists(string dbUser, string dbPass, string database, string dbServer, string table);
};