#ifndef __ISS_MY_SQL__
#define __ISS_MY_SQL__

#include "Utility.h"

#include <mysql_connection.h>
#include <mysql_driver.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h> 

class ISS_API MySQL {
public:
	~MySQL() {
		delete connection;
		delete statement;
	}
public:
	Void connect(StringC host,
		         StringC user,
				 StringC pwrd);
public:
	sql::ResultSet* query(StringC query, ...);
	Void update(StringC query, ...);
	sql::PreparedStatement* prepare(StringC query, ...);
public:
	inline sql::Statement* operator -> () {
		try {
			return this->statement;
		} catch (sql::SQLException& e) {
			throw Exception("MySQL : %s", e.what());
		}
	}
private:
	sql::Driver* driver = NULL;
	sql::Connection* connection = NULL;
	sql::Statement* statement = NULL;
private:
	static Sint8 queryBuffer[4096];
};

#endif // ~__ISS_MY_SQL__