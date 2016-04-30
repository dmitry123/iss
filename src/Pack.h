#ifndef __ISS_PACK__
#define __ISS_PACK__

#include "Extension.h"
#include "Utility.h"

#include <fstream>

class ISS_API PackStream {
public:
	PackStream() {
	}
public:
	Void write(VoidP bufferData, Uint32 size);
	Void read(VoidP bufferData, Uint32 size);
	Void seek(Uint32 position);
public:
	inline operator Bool () const {
		return !this->iStream_;
	}
public:
	inline std::ifstream* getInputStream() {
		return &this->iStream_;
	}
	inline std::ofstream* getOutputStream() {
		return &this->oStream_;
	}
private:
	std::ifstream iStream_;
	std::ofstream oStream_;
};

class ISS_API PackLoader {
	friend class Image;
private:
	PackLoader(ImagePtr image) :
		image_(image)
	{
		this->_declare<HashGenerator>();
		this->_declare<HistCalculator>();
		this->_declare<FaceDetector>();
		this->_declare<PreviewMaker>();
		this->_declare<FrameMaker>();
	}
private:
	~PackLoader() {
		for (ImageExtension* ie : this->imageExtensionList_) {
			delete ie;
		}
	}
public:
	inline PackLoader& open(const std::string& fileName) {
		return this->open(fileName.data());
	}
	inline PackLoader& load(const std::string& fileName) {
		return this->load(fileName.data());
	}
	inline PackLoader& save(const std::string& fileName) {
		return this->save(fileName.data());
	}
public:
	PackLoader& open(StringC fileName);
	PackLoader& load(StringC fileName);
	PackLoader& save(StringC fileName);
public:
	PackLoader& process(Void);
public:
	inline PackLoaderPtr operator -> () {
		return this;
	}
public:
	inline std::vector<ImageExtension*>& getExtensionList() {
		return this->imageExtensionList_;
	}
public:
	template <class Extension> inline ImageExtension* getExtension() {
		for (ImageExtension* ie : this->imageExtensionList_) {
			if (typeid(*ie) == typeid(Extension)) {
				return ie;
			}
		}
		this->_throwExtensionError(typeid(Extension).name());
	}
private:
	Void _throwExtensionError(StringC typeName);
private:
	template <class Extension> Void _declare() {
		this->imageExtensionList_.push_back(
			new Extension(this->image_, &this->stream_));
	}
private:
	PackStream stream_;
	ImagePtr image_;
private:
	std::vector < ImageExtension* >
		imageExtensionList_;
};

#endif // ~__ISS_PACK__