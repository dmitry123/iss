#include "MySQL.h"

void MySQL::connect(StringC host, StringC user, StringC pwrd) {

	this->driver = get_driver_instance();

	try {
		this->connection = this->driver->connect(host, user, pwrd);
		this->statement = this->connection->createStatement();
	}
	catch (sql::SQLException& e) {
		throw Exception("MySQL : [%d] %s", e.getErrorCode(), e.what());
	}
}

sql::ResultSet* MySQL::query(StringC query, ...) {

	va_list vaList;
	va_start(vaList, query);
	vsprintf(queryBuffer, query, vaList);
	va_end(vaList);

	if (!this->statement) {
		throw Exception("MySQL : Not connected");
	}

	try {
		return this->statement->executeQuery(
			queryBuffer);
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() > 0) {
			throw Exception("MySQL : [%d] %s", e.getErrorCode(), e.what());
		}
	}

	return NULL;
}

void MySQL::update(StringC query, ...) {

	va_list vaList;
	va_start(vaList, query);
	vsprintf(queryBuffer, query, vaList);
	va_end(vaList);

	if (!this->statement) {
		throw Exception("MySQL : Not connected");
	}

	try {
		this->statement->executeUpdate(
			queryBuffer);
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() > 0) {
			throw Exception("MySQL : [%d] %s", e.getErrorCode(), e.what());
		}
	}
}

sql::PreparedStatement* MySQL::prepare(StringC query, ...) {

	va_list vaList;
	va_start(vaList, query);
	vsprintf(queryBuffer, query, vaList);
	va_end(vaList);

	if (!this->statement) {
		throw Exception("MySQL : Not connected");
	}

	try {
		return this->connection->prepareStatement(
			queryBuffer);
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() > 0) {
			throw Exception("MySQL : [%d] %s", e.getErrorCode(), e.what());
		}
	}

	return NULL;
}

Sint8 MySQL::queryBuffer[4096];