#pragma once
#include <log4cplus/config.hxx>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/spi/factory.h>
#include <log4cplus/spi/loggingevent.h>
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

namespace log4cplus
{
	class LOG4CPLUS_EXPORT DatabaseAppender : public Appender
	{
		private:
			bool validConnection;
			sql::Connection *dbConnection;
			sql::Driver *dbDriver;

			void connectToDB();
			bool checkAndCreateTable();

		public:
			// Ctors
			DatabaseAppender(string databaseServer, string database, string databaseUser, string databasePassword);
			DatabaseAppender(helpers::Properties const & properties);

			//Dtor	
			~DatabaseAppender();			

			virtual void close();

		protected:
			void append(spi::InternalLoggingEvent const & ev);
				

			// Data
			string dbServer;
			string db;
			string dbUser;
			string dbPassword;			
	};
}