#ifndef __ISS_USER__
#define __ISS_USER__

#include "Utility.h"

typedef Sint8 PasswordHash[33];

class ISS_API User {
public:
	StringC userName;
	Uint32 nameHash;
	PasswordHash userPassword;
public:
	User() :
		userName(0),
		nameHash(0)
	{
	}
	User(StringC userName, StringC userPassword) :
		userName(userName)
	{
		this->set(userName, userPassword);
	}
public:
	Void set(StringC userName,
		     StringC userPassword);
};

class UserSqlNode {
public:
	Buffer userName;
	Buffer passwordHash;
	Uint32 userID;
public:
	UserSqlNode(Buffer userName,
		        Buffer passwordHash,
		        Uint32 userID) :
		userName(userName),
		passwordHash(passwordHash),
		userID(userID)
	{
	}
};

#endif // ~__ISS_USER__