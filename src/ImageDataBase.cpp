#include "ImageDataBase.h"

Void ImageDataBase::add(Image* imagePtr) {

	if (this->contains(imagePtr)) {
		this->mySql.update("UPDATE %s SET refCount=refCount+1 WHERE fileName='%s'", this->getName(),
			imagePtr->md5);
	} else {
		this->mySql.query("INSERT INTO %s VALUES('%s', CURDATE(), %lld, 1, %d)", this->getName(),
			imagePtr->md5, imagePtr->hash, imagePtr->faces.size());
	}
}

Void ImageDataBase::erase(Image* imagePtr) {

	sql::ResultSet* result;

	result = this->mySql.query("SELECT * FROM %s WHERE fileName='%s'", this->getName(),
		imagePtr->md5);

	if (result->next()) {
		if (result->getInt("refCount") != 1) {
			this->mySql.update("UPDATE %s SET refCount=refCount-1 WHERE fileName='%s'", this->getName(),
				imagePtr->md5);
		} else {
			this->mySql.query("DELETE FROM %s WHERE fileName='%s'",
				this->getName(), imagePtr->md5);
		}
	}
}

Bool ImageDataBase::contains(Image* imagePtr) {

	sql::ResultSet* result = this->mySql.query("SELECT * FROM %s WHERE fileName='%s'",
		this->getName(), imagePtr->md5);

	if (result->next()) {
		return TRUE;
	}

	return FALSE;
}

Void ImageDataBase::list(Vector<ImageSqlNode>& resultList, StringC whereQuery) {

	sql::ResultSet* result;

	if (whereQuery) {
		result = this->mySql.query("SELECT * FROM %s WHERE %s",
			this->getName(), whereQuery);
	} else {
		result = this->mySql.query("SELECT * FROM %s",
			this->getName());
	}

	if (resultList.capacity() < result->rowsCount()) {
		throw Exception("ImageDataBase : Need more (%d) blocks for vector", result->rowsCount() - resultList.capacity());
	}

	while (result->next()) {
		resultList.push_back(ImageSqlNode(
			Buffer(result->getString("fileName")),
			Hash64(result->getUInt64("imageHash")),
			Uint32(result->getInt("refCount"))
		));
	}
}

Void ImageDataBase::update(Image* imagePtr) {
	throw Exception("ImageDataBase : Method not-implemented (%s)", __FUNCTION__);
}