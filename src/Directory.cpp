#include "Directory.h"

#ifdef ISS_WINDOWS
#  include <Windows.h>
#else
#  include <dirent.h>
#  include <unistd.h>
#endif

#include <sys/stat.h>

#ifdef ISS_WINDOWS
#  define __FindFirstFile(_dirname) FindFirstFileA(_dirname, &file)
#  define __FindNextFile(_handle) FindNextFileA(_handle, &file)
#  define __FindClose(_handle) FindClose(_handle)
#else
#  define __FindFirstFile(_dirname) opendir(_dirname)
#  define __FindNextFile(_handle) readdir((DIR*)_handle)
#  define __FindClose(_handle) closedir((DIR*)_handle) == 0
#endif

#ifdef ISS_WINDOWS
#  undef GetMessage
#  undef GetCurrentDirectory
#endif

#ifdef ISS_WINDOWS
#  define FLAG_HIDDEN FILE_ATTRIBUTE_HIDDEN
#  define FLAG_RWX (S_IREAD | S_IWRITE | S_IEXEC)
#  define FLAG_READABLE S_IREAD 
#  define FLAG_WRITABLE S_IWRITE
#  define FLAG_EXECUTABE S_IEXEC
#  define FLAG_SYSTEM FILE_ATTRIBUTE_SYSTEM
#else
#  define FLAG_HIDDEN UF_HIDDEN
#  define FLAG_RWX S_IRWXU
#  define FLAG_READABLE S_IRUSR
#  define FLAG_WRITABLE S_IWUSR
#  define FLAG_EXECUTABE S_IXUSR
#  define FLAG_SYSTEM UF_IMMUTABLE
#endif

typedef struct stat Stat;

#ifdef ISS_WINDOWS
typedef BOOL Dirent;
#else
typedef struct dirent* Dirent;
#endif

#ifdef ISS_WINDOWS
#  define strcasecmp _stricmp
#endif

#ifndef ISS_WINDOWS
#  define INVALID_HANDLE_VALUE (VoidP)-1
#endif

Directory::Directory() {

	this->handle_ = 0;
	this->flags_ = 0;
	this->extfilter_ = 0;

	memset(this->path_, 0, sizeof(this->path_));
	memset(this->last_, 0, sizeof(this->last_));
}

Directory::~Directory() {

	if (this->handle_) {
		this->close();
	}
}

Void Directory::open(StringC path, Uint32 flags) {

	Sint8 searchPath[0xff];

#ifdef ISS_WINDOWS
	WIN32_FIND_DATAA file;
#endif

	if (this->handle_) {
		this->close();
	}

	if (!flags) {
		flags = kDirectories | kFiles;
	}

	if (!path || path[0] == '\0') {
		throw Exception("Directory is NULL");
	}

	strcpy(searchPath, path);

#ifdef ISS_WINDOWS
	strcat(searchPath, "\\*.*");
#endif

	this->handle_ = __FindFirstFile(searchPath);

	if (!this->handle_ || this->handle_ == INVALID_HANDLE_VALUE) {
		throw Exception("Unable to open directory (%s)", path);
	}

	if (!(this->flags_ & kDots)) {
		__FindNextFile(this->handle_);
	}

	strcpy(this->path_, path);

	this->flags_ = flags;
}

StringC Directory::next(Void) {

#ifdef ISS_WINDOWS
	WIN32_FIND_DATAA file;
#endif

	Dirent entry;
	Stat st;
	Uint32 flags;

	if (!this->handle_) {
		return NULL;
	}

__RepeatFileSearch:;

	memset(this->last_, 0, sizeof(this->last_));

	if (!(entry = __FindNextFile(this->handle_))) {

		Uint32 flags = this->flags_;

		if (!this->stack_.size()) {
			return NULL;
		}

		this->close();

	__TryToOpenAnotherDirectory:

		try {
			this->open(this->stack_.back(), flags);
		}
		catch (Exception error) {

			this->stack_.pop_back();

			if (this->stack_.size()) {
				goto __TryToOpenAnotherDirectory;
			}
			else {
				return NULL;
			}
		}

		if (this->stack_.size() == 0) {
			return NULL;
		}

		this->stack_.pop_back();

		goto __RepeatFileSearch;
	}

	strcpy(this->last_, this->path_);

	if (this->last_[strlen(this->last_) - 1] != '/') {
		this->last_[strlen(this->last_)] = '/';
	}

	StringC filename;

#ifdef ISS_WINDOWS
	filename = file.cFileName;
	flags = file.dwFileAttributes;
#else
	filename = entry->d_name;
#endif

	strcat(this->last_, filename);

	if (!(this->flags_ & kSystem) && *filename == '.') {
		goto __RepeatFileSearch;
	}

	if (stat(this->last_, &st) == 0) {

#ifndef ISS_WINDOWS
		flags = st.st_flags;
#endif

		// check content for hidden
		if (flags & FLAG_HIDDEN && !(this->flags_ & kHidden)) {
			goto __RepeatFileSearch;
		}

		// check content for system
		if (flags & FLAG_SYSTEM && !(this->flags_ & kSystem)) {
			goto __RepeatFileSearch;
		}

		// check content for dir
		if (st.st_mode & S_IFDIR) {

			// check content for deep
			if (this->flags_ & kDeep) {
				this->stack_.push_back(this->last_);
			}

			// check dir for dirs
			if (!(this->flags_ & kDirectories)) {
				goto __RepeatFileSearch;
			}
		}
		else {

			// check dir for files
			if (!(this->flags_ & kFiles)) {
				goto __RepeatFileSearch;
			}
			else {

				// check file for RWX
				if (st.st_mode & FLAG_RWX && this->flags_ & kExecutable &&
					this->flags_ & kWritable &&
					this->flags_ & kReadble) {
					goto __RepeatFileSearch;
				}

				// check for R
				if (st.st_mode & FLAG_READABLE && this->flags_ & kReadble) {
					goto __RepeatFileSearch;
				}

				// check for W
				if (st.st_mode & FLAG_WRITABLE && this->flags_ & kWritable) {
					goto __RepeatFileSearch;;
				}

				// check for E
				if (st.st_mode & FLAG_EXECUTABE && this->flags_ & kExecutable) {
					goto __RepeatFileSearch;
				}
			}
		}
	}

	return this->last_;
}

Void Directory::close(Void) {

	if (this->handle_) {
		__FindClose(this->handle_);
	}

	this->handle_ = 0;
	this->flags_ = 0;

	memset(this->path_, 0, sizeof(this->path_));
	memset(this->last_, 0, sizeof(this->last_));
}

List <Buffer> Directory::getList(Void) {

	List <Buffer> list;

	while (this->next()) {
		list.push_back(this->getLast());
	}

	return list;
}

static List <Buffer> __GetContentAtPath(StringC path, Uint32 flags) {

	Directory dir;
	List <Buffer> list;

	dir.open(path, flags);
	while (dir.next()) {
		list.push_back(dir.getLast());
	}
	dir.close();

	return list;
}

List <Buffer> Directory::getContent(StringC path) {
	return __GetContentAtPath(path, Directory::kFiles);
}

List <Buffer> Directory::getDirectories(StringC path) {
	return __GetContentAtPath(path, Directory::kDirectories);
}

List <Buffer> Directory::getContentD(StringC path) {
	return __GetContentAtPath(path, Directory::kFiles | Directory::kDirectories | Directory::kDeep);
}

List <Buffer> Directory::getDirectoriesD(StringC path) {
	return __GetContentAtPath(path, Directory::kDirectories | Directory::kDeep);
}