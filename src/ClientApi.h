#ifndef __ISS_CLIENT_API__
#define __ISS_CLIENT_API__

#include "Utility.h"
#include "JsonBuilder.h"
#include "MySQL.h"

class ISS_API ClientApi {
public:
	typedef Void(*RunProc)(ClientApi*);
public:
	ClientApi(Sint32 argc, String* argv) :
		argc(argc),
		argv(argv)
	{
	}
public:
	Void kill(Uint32 returnCode = 0);
	Void error(StringC message, ...);
	Void warning(StringC message, ...);
	Void run(RunProc runProc);
	Void connect(StringC host = NULL);
public:
	JsonBuilder* getJson() {
		return &this->jsonResponse;
	}
	MySQL* getMySql() {
		return &this->mySql;
	}
public:
	Sint32 argc;
	String* argv;
private:
	JsonBuilder jsonResponse;
	MySQL mySql;
	Uint32 elapsedTime;
};

#endif // ~__ISS_CLIENT_API__