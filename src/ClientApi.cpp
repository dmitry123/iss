#include "ClientApi.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <opencv2/opencv.hpp>

Void ClientApi::kill(Uint32 returnCode) {

	this->elapsedTime = clock() - this->elapsedTime;
	this->jsonResponse.add("responseTime", this->elapsedTime);

	Buffer appName(this->argv[0]);

	for (Sint8& c : appName) {
		if (c == '\\') {
			c = '/';
		}
		c = tolower(c);
	}

	StringC ptrFileName = appName.data();
	StringC baseFileName = appName.data();

	while ((ptrFileName = strchr(ptrFileName + 1, '/'))) {
		baseFileName = ptrFileName + 1;
	}
	this->jsonResponse.add("executableFile", baseFileName);

	printf(this->jsonResponse.build().data());
	exit(returnCode);
}

Void ClientApi::error(StringC message, ...) {

	va_list vaList;
	Sint8 errorBuffer[2048];

	va_start(vaList, message);
	vsprintf(errorBuffer, message, vaList);
	va_end(vaList);

	this->jsonResponse.add("errorMessage", errorBuffer);
	this->kill(1);
}

Void ClientApi::warning(StringC message, ...) {

	va_list vaList;
	Sint8 errorBuffer[2048];

	va_start(vaList, message);
	vsprintf(errorBuffer, message, vaList);
	va_end(vaList);

	this->jsonResponse.add("warningMessage", errorBuffer);
}

Void ClientApi::run(RunProc runProc) {

	try {
		this->connect();
		elapsedTime = clock();
		runProc(this);
		this->kill();
	}
	catch (sql::MethodNotImplementedException& e) {
		this->error(e.what());
	}
	catch (sql::InvalidArgumentException& e) {
		this->error(e.what());
	}
	catch (sql::InvalidInstanceException& e) {
		this->error(e.what());
	}
	catch (sql::NonScrollableException& e) {
		this->error(e.what());
	}
	catch (Exception e) {
		this->error(e.what());
	}
	catch (cv::Exception& e) {
		this->error(e.what());
	}
	catch (sql::SQLException& e) {
		this->error(e.what());
	}
}

Void ClientApi::connect(StringC host) {

	if (!host) {
		host = "tcp://localhost/iss";
	}

	this->mySql.connect(host, "root", "");
}