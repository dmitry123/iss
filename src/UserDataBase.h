#ifndef __ISS_USER_DATA_BASE__
#define __ISS_USER_DATA_BASE__

#include "DataBase.h"
#include "User.h"
#include "Hash.h"

class ISS_API UserDataBase : public DataBase <User, UserSqlNode> {
public:
	UserDataBase(MySQL& mySql)
		: DataBase(mySql, "udb")
	{
	}
public:
	Void add(User* userPtr);
	Bool contains(User* userPtr);
	Bool validate(User* userPtr);
	Void erase(User* userPtr);
	Void update(User* userPtr);
public:
	Void list(Vector<UserSqlNode>& resultList,
		StringC whereQuery = NULL);
};

#endif // ~__ISS_USER_DATA_BASE__