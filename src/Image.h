#ifndef __ISS_IMAGE__
#define __ISS_IMAGE__

#include "Utility.h"
#include "Hash.h"

#include <opencv2/opencv.hpp>
#include <libface/face.h>

// libface's face typedef
typedef libface::Face Face;

// opencv matrix
typedef cv::Mat Frame;

// rgb and hsv histogramms
typedef Float32 HistRgb[255], HistRgbPtr;
typedef Float32 HistHsv[360], HistHsvPtr;

// md5 hash summ
typedef Sint8 MD5Hash[33];

// list with faces
typedef std::vector<Frame> FaceList;

class ISS_API HistInfo {
public:
	HistInfo() {
		memset(this, 0, sizeof(HistInfo));
	}
public:
	HistRgb red;
	HistRgb green;
	HistRgb blue;
	HistHsv hue;
	HistHsv saturation;
	HistHsv value;
};

class ISS_API ImageInfo {
	friend class PreviewMaker;
	friend class FrameMaker;
	friend class FaceDetector;
	friend class Image;
public:
	inline Uint32 getWidth()  const { return this->width; }
	inline Uint32 getHeight() const { return this->height; }
	inline Uint32 getDepth()  const { return this->depth; }
	inline Uint32 getBytes()  const { return this->bytes; }
	inline Uint32 getSize()   const { return this->size; }
public:
	inline Frame* getFrame() {
		return &this->frame;
	}
public:
	inline Void setWidth(Uint32 value) {
		this->width = value;
		this->size_();
	}
	inline Void setHeight(Uint32 value) {
		this->height = value;
		this->size_();
	}
	inline Void setDepth(Uint32 value) {
		this->depth = value;
		this->bytes = value / 8;
		this->size_();
	}
	inline Void setBytes(Uint32 value) {
		this->bytes = value;
		this->depth = value * 8;
		this->size_();
	}
public:
	inline Uint32 getSizeOf() {
		return sizeof(ImageInfo) - sizeof(Frame);
	}
private:
	inline Void size_() {
		this->size = this->width * this->height * this->bytes;
	}
private:
	Uint32 width = 0;
	Uint32 height = 0;
	Uint32 depth = 0;
	Uint32 bytes = 0;
	Uint32 size = 0;
protected:
	Frame frame;
};

class ISS_API Image {
	friend class PackLoader;
public:
	Hash64    hash;
	ImageInfo image;
	ImageInfo preview;
	HistInfo  hist;
	FaceList  faces;
	MD5Hash   md5;
public:
	Image();
	~Image();
public:
	Image(StringC fileName) : Image() {
		this->load(fileName);
	}
	Image(const std::string& fileName) : Image() {
		this->load(fileName);
	}
public:
	inline Void load(const std::string& fileName) {
		this->load(fileName.data());
	}
	inline Void save(const std::string& fileName) {
		this->save(fileName.data());
	}
public:
	Void load(StringC fileName);
	Void save(StringC fileName);
public:
	inline PackLoaderPtr getLoader() {
		return this->packLoader_;
	}
private:
	PackLoaderPtr packLoader_;
};

class ISS_API ImageSqlNode {
	friend class SearchEngine;
public:
	ImageSqlNode() {
		// ignore
	}
public:
	ImageSqlNode(Buffer fileName, Hash64 imageHash, Uint32 refCount) :
		fileName(fileName),
		imageHash(imageHash),
		refCount(refCount)
	{
	}
public:
	Buffer fileName;
	Hash64 imageHash;
	Uint32 refCount;
};

#endif // ~__ISS_IMAGE__