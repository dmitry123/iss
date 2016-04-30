#ifndef __ISS_IMAGE_DATA_BASE__
#define __ISS_IMAGE_DATA_BASE__

#include "Utility.h"
#include "DataBase.h"
#include "Image.h"
#include "User.h"

class ISS_API ImageDataBase : public DataBase<Image, ImageSqlNode> {
public:
	ImageDataBase(MySQL& mySql)
		: DataBase(mySql, "idb")
	{
	}
public:
	Void add(Image* imagePtr);
	Bool contains(Image* imagePtr);
	Void erase(Image* imagePtr);
	Void update(Image* imagePtr);
public:
	Void list(Vector<ImageSqlNode>& resultList,
		StringC whereQuery = NULL);
};

#endif // ~__ISS_IMAGE_DATA_BASE__