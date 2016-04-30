#ifndef __ISS_DATA_BASE__
#define __ISS_DATA_BASE__

#include "MySQL.h"

template <class T, class N, class E = StringC> class DataBase {
public:
	DataBase(MySQL& mySql, StringC tableName) :
		mySql(mySql),
		tableName(tableName)
	{
	}
	virtual ~DataBase() {
	}
protected:
	MySQL& mySql;
public:
	virtual Void add(T*) = 0;
	virtual Bool contains(T*) = 0;
	virtual Void erase(T*) = 0;
	virtual Void update(T*) = 0;
public:
	virtual Void list(Vector<N>& resultList,
		StringC whereQuery = NULL) = 0;
	virtual Uint32 size(
		StringC whereQuery = NULL);
public:
	inline StringC getName() const {
		return this->tableName;
	}
	inline MySQL* getMySql() {
		return &this->mySql;
	}
private:
	StringC tableName;
};

template <class T, class N, class E = StringC>
Uint32 DataBase<T, N, E>::size(StringC whereQuery) {

	sql::ResultSet* result;

	if (whereQuery) {
		result = this->mySql.query("SELECT COUNT(*) FROM %s WHERE %s",
			this->getName(), whereQuery);
	}
	else {
		result = this->mySql.query("SELECT COUNT(*) FROM %s",
			this->getName());
	}

	if (result->next()) {
		return result->getInt(1);
	}

	return 0;
}

#endif // ~__ISS_DATA_BASE__