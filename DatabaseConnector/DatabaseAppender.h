#pragma once
#include <log4cplus/config.hxx>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/spi/factory.h>
#include <log4cplus/spi/loggingevent.h>
#include <string>
#include <mysql/mysql.h>
#include <mysql/my_global.h>
#include <sstream>
#include <string.h>
#include <time.h>

using namespace std;

namespace log4cplus
{
	class LOG4CPLUS_EXPORT DatabaseAppender : public Appender
	{
		private:
			bool validConnection;
			MYSQL *dbConnection;
			
			void connectToDB();
			bool checkAndCreateTable();

		public:
			// Ctors
			DatabaseAppender(string databaseServer, string database, string databaseUser, string databasePassword);
			DatabaseAppender(helpers::Properties const & properties);

			//Dtor	
			~DatabaseAppender();			

		protected:
			void append(spi::InternalLoggingEvent const & ev);
				

			// Data
			string dbServer;
			string db;
			string dbUser;
			string dbPassword;
			int dbPort;
	};
}