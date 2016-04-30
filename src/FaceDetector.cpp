#include "Extension.h"
#include "Pack.h"
#include "Image.h"

#include <libface/LibFace.h>

Void FaceDetector::process() {

	// create new lib static frace object
	// which will help us to detect faces.
	// this library based on opencv, so
	// it simply automaticall loads haar
	// cascades and seeking faces with
	// 5 or 7 cascades with eyes and mouth.
	// image also has comparative, but i've
	// used my own
	static libface::LibFace libFace =
		libface::LibFace(libface::DETECT, ".");

	// get image's frame
	cv::Mat* frame = this->getImage()->image.getFrame();

	// detect all faces and save in temporary vector
	std::vector<libface::Face> faceList = libFace.detectFaces(
		&(IplImage)*frame, frame->size());

	// clear current face array
	this->getImage()->faces.clear();

	// copy to image's frame
	for (libface::Face& f : faceList) {

		// matrix with face
		cv::Mat matFace;

		// get face at detected position
		// and copy to new matrix
		this->getImage()->image.frame(
			cv::Rect(f.getX1(), f.getY1(), f.getWidth(), f.getHeight())
		).copyTo(matFace);

		// resize matrix and convert
		// to grayscale color mode
		cv::resize(matFace, matFace, cv::Size(FRAME_WIDTH, FRAME_HEIGHT));
		cv::cvtColor(matFace, matFace, cv::COLOR_RGB2GRAY);

		// push matrix to list
		this->getImage()->faces.push_back(matFace);
	}
}

Void FaceDetector::read() {

	Uint32 faceCount = 0;
	Uint32 faceSize = 0;

	// jump to out position
	this->getStream()->seek(this->getPosition());

	// read face count from stream
	this->getStream()->read(&faceCount, sizeof(faceCount));

	while (faceCount--) {

		// read compressed face size from stream
		this->getStream()->read(&faceSize, sizeof(faceSize));

		// array for compressed frame
		std::vector<uchar> compressedFrame(faceSize);

		// read compressed face from stream
		this->getStream()->read(compressedFrame.data(), faceSize);

		// decode face to get original frame
		cv::Mat faceMat = cv::imdecode(compressedFrame,
			cv::IMREAD_GRAYSCALE);

		// push matrix to list
		this->getImage()->faces.push_back(faceMat);
	}
}

Uint32 FaceDetector::size() {

	Uint32 size = 0;

	// write face count to stream
	size += sizeof(Uint32);

	// temporary face list
	std::vector<Frame> tempFaces = this->getImage()->faces;

	// look though all faces in image
	for (Frame& f : tempFaces) {

		// array for compressed frame
		std::vector<uchar> compressedFrame;

		// compress preview image with
		// jpeg encoder
		cv::imencode(".jpg", f, compressedFrame);

		// write encoded frame's size to stream
		size += sizeof(Uint32);

		// write compressed face to stream
		size += compressedFrame.size();
	}

	ISS_TODO("Need optimization!");

	return size;
}

Void FaceDetector::write() {

	// get count of faces
	Uint32 faceCount = this->getImage()->faces.size();

	// write face count to stream
	this->getStream()->write(&faceCount, sizeof(faceCount));

	// temporary face list
	std::vector<Frame> tempFaces = this->getImage()->faces;

	// look though all faces in image
	for (Frame& f : tempFaces) {

		// array for compressed frame
		std::vector<uchar> compressedFrame;

		// compress preview image with
		// jpeg encoder
		cv::imencode(".jpg", f, compressedFrame);

		// compressed face's size
		Uint32 faceSize = compressedFrame.size();

		// write encoded frame's size to stream
		this->getStream()->write(&faceSize, sizeof(faceSize));

		// write compressed face to stream
		this->getStream()->write(compressedFrame.data(),
			compressedFrame.size());
	}
}