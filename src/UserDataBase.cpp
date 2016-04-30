#include "UserDataBase.h"

#include <Windows.h>
#include <WinHttp.h>

#pragma comment(lib, "WinHttp.lib")

typedef wchar_t* StringW, *const StringWC;

Void sendPostRequest(StringWC host, StringWC urlPage, StringC message, ...) {

	HINTERNET hSession = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;

	va_list vaList;
	Sint8 buffer[1024];
	Uint32 length;

	va_start(vaList, message);
	length = vsprintf(buffer, message, vaList);
	va_end(vaList);

	hSession = WinHttpOpen(L"HTTP/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

	hConnect = WinHttpConnect(hSession, host,
		INTERNET_DEFAULT_HTTP_PORT, 0);

	hRequest = WinHttpOpenRequest(hConnect, L"POST", urlPage,
		NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

	StringWC additionHeader = L"Content-Type: application/x-www-form-urlencoded\r\n";

	if (!WinHttpSendRequest(hRequest, additionHeader,
		wcslen(additionHeader), buffer, length, length, 0)
		) {
		throw Exception("Post : Unable to register user, HTTP error");
	}

	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);
}

Void UserDataBase::add(User* userPtr) {

	if (this->contains(userPtr)) {
		throw Exception("UserDataBase : User with that name already exists (%s)", userPtr->userName);
	}

#if 1
	sendPostRequest(L"127.0.0.1", L"iss/php/RegisterUser.php",
		"userName=%s&userPassword=%s", "dmitry", "dmitry");
#else
	this->mySql.query("INSERT INTO %s (userName, passwordHash) VALUES('%s', '%s')", this->getName(),
		userPtr->userName, userPtr->userPassword);
#endif
}

Bool UserDataBase::contains(User* userPtr) {

	sql::ResultSet* result = this->mySql.query("SELECT * FROM %s WHERE userName='%s'",
		this->getName(), userPtr->userName);

	if (result->next()) {
		return TRUE;
	}

	return FALSE;
}

Bool UserDataBase::validate(User* userPtr) {

	sql::ResultSet* result = this->mySql.query("SELECT * FROM %s WHERE userName='%s' AND passwordHash='%s'",
		this->getName(), userPtr->userName, userPtr->userPassword);

	if (result->next()) {
		return TRUE;
	}

	return FALSE;
}

Void UserDataBase::erase(User* userPtr) {

	this->mySql.query("DELETE FROM %s WHERE userName=%s", this->getName(),
		userPtr->userName);
}

Void UserDataBase::update(User* userPtr) {

	this->mySql.update("UPDATE %s SET userName='%s' passwordHash='%s' WHERE userName='%s'", this->getName(),
		userPtr->userName, userPtr->userPassword, userPtr->userName);
}

Void UserDataBase::list(Vector<UserSqlNode>& resultList, StringC whereQuery) {

	sql::ResultSet* result;

	if (whereQuery) {
		result = this->mySql.query("SELECT * FROM %s WHERE %s",
			this->getName(), whereQuery);
	}
	else {
		result = this->mySql.query("SELECT * FROM %s",
			this->getName());
	}

	if (resultList.capacity() < result->rowsCount()) {
		throw Exception("UserDataBase : Need more (%d) blocks for std::vector", result->rowsCount() - resultList.capacity());
	}

	while (result->next()) {
		resultList.push_back(UserSqlNode(
			Buffer(result->getString("userName")),
			Buffer(result->getString("passwordHash")),
			Uint32(result->getInt("userID"))
		));
	}
}