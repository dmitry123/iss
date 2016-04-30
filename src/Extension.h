#ifndef __ISS_EXTENSION__
#define __ISS_EXTENSION__

#include "Utility.h"

class ISS_API ImageExtension {
	friend class PackLoader;
public:
	ImageExtension(ImagePtr image, PackStreamPtr stream) :
		image_(image),
		stream_(stream)
	{
		this->position_ = 0;
		this->size_ = 0;
		this->processed_ = 0;
	}
public:
	Void process(Bool processed) {
		this->processed_ = TRUE; this->process();
	}
public:
	virtual Void process() {
		this->processed_ = FALSE;
	}
public:
	virtual Void read() = 0;
	virtual Uint32 size() = 0;
	virtual Void write() = 0;
public:
	inline ImagePtr getImage() {
		return this->image_;
	}
	inline PackStreamPtr getStream() {
		return this->stream_;
	}
	inline Uint32 getPosition() {
		return this->position_;
	}
	inline Uint32 getSize() {
		return this->size_;
	}
private:
	ImagePtr image_;
	PackStreamPtr stream_;
	Uint32 position_;
	Uint32 size_;
	Bool processed_;
};

class ISS_API HashGenerator : public ImageExtension {
public:
	HashGenerator(ImagePtr image, PackStreamPtr stream) :
		ImageExtension(image, stream)
	{
	}
public:
	Void process();
	Void read();
	Uint32 size();
	Void write();
};

class ISS_API HistCalculator : public ImageExtension {
public:
	HistCalculator(ImagePtr image, PackStreamPtr stream) :
		ImageExtension(image, stream)
	{
	}
public:
	Void process();
	Void read();
	Uint32 size();
	Void write();
};

class ISS_API FaceDetector : public ImageExtension {
public:
	enum {
		FRAME_WIDTH = 128,
		FRAME_HEIGHT = 128,
		FRAME_DEPTH = 24,
		FRAME_SIZE =
		FRAME_WIDTH  *
		FRAME_HEIGHT *
		FRAME_DEPTH / 24
	};
public:
	FaceDetector(ImagePtr image, PackStreamPtr stream) :
		ImageExtension(image, stream)
	{
	}
public:
	Void process();
	Void read();
	Uint32 size();
	Void write();
};

class ISS_API PreviewMaker : public ImageExtension {
public:
	enum {
		FRAME_WIDTH = 256,
		FRAME_HEIGHT = 256,
		FRAME_DEPTH = 24,
		FRAME_SIZE =
		FRAME_WIDTH  *
		FRAME_HEIGHT *
		FRAME_DEPTH / 8
	};
public:
	PreviewMaker(ImagePtr image, PackStreamPtr stream) :
		ImageExtension(image, stream)
	{
	}
public:
	Void process();
	Void read();
	Uint32 size();
	Void write();
};

class ISS_API FrameMaker : public ImageExtension {
public:
	FrameMaker(ImagePtr image, PackStreamPtr stream) :
		ImageExtension(image, stream)
	{
	}
public:
	Void read();
	Uint32 size();
	Void write();
};

#endif // ~__ISS_EXTENSION__