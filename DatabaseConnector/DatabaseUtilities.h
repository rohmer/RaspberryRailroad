#pragma once
#include <string>
#include <sstream>
#include <string.h>
#include <time.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class DatabaseUtilities
{
	private:
		static sql::Connection *getDBConnection(string dbUser, string dbPass, string database, string dbServer);		

	public:
		static bool ClearTable(string dbUser, string dbPass, string database, string dbServer, string table);
};