#ifndef __LAME_CORE__DIRECTORY__
#define __LAME_CORE__DIRECTORY__

#include "Define.h"
#include "Utility.h"

class ISS_API Directory {
public:
	typedef enum  {
		kDefault = 0x0000,
		kFiles = 0x0001,
		kDirectories = 0x0002,
		kDots = 0x0004,
		kDeep = 0x0008,
		kReadble = 0x0010,
		kWritable = 0x0020,
		kExecutable = 0x0040,
		kHidden = 0x0080,
		kSystem = 0x0100
	} FilterFlags;
public:
	typedef class Fpc {
	public:
		inline Fpc() {
			memset(this->path_, 0, sizeof(this->path_));
		}
		inline Fpc(StringC path) {
			strcpy(this->path_, path);
		}
		inline Void operator = (StringC path) {
			strcpy(this->path_, path);
		}
	public:
		inline operator String () { return this->path_; }
		inline operator StringC () { return this->path_; }
	private:
		Sint8 path_[0xff];
	} Fpc;
public:
	typedef List <Fpc> Stack;
public:
	Void open(StringC path, Uint32 flags);
	StringC next(Void);
	Void close(Void);
	List <Buffer> getList(Void);
public:
	inline StringC getLast(Void) const {
		return this->last_;
	}
public:
	 Directory();
	~Directory();
public:
	static List <Buffer> getContent(StringC path);
	static List <Buffer> getDirectories(StringC path);
	static List <Buffer> getContentD(StringC path);
	static List <Buffer> getDirectoriesD(StringC path);
private:
	VoidP handle_;
	Sint8 path_[0xff];
	Uint32 flags_;
	Sint8 last_[0xff];
	Stack stack_;
	StringC extfilter_;
	List <Buffer> extlist_;
};

#endif // ~__LAME_CORE__DIRECTORY__