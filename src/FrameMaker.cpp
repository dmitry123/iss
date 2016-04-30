#include "Extension.h"
#include "Pack.h"
#include "Image.h"

Void FrameMaker::read() {

	// array for compressed frame
	std::vector<uchar> compressedFrame(this->getSize());

	// jump to out position
	this->getStream()->seek(this->getPosition());

	// read compressed data from pack
	this->getStream()->read(compressedFrame.data(),
		compressedFrame.size());

	// decode preview data
	this->getImage()->image.frame = cv::imdecode(
		compressedFrame, cv::IMREAD_COLOR);

	// save preview parameters likes
	// width, height, depth, size (auto-compute)
	this->getImage()->image.setWidth(this->getImage()->image.frame.size().width);
	this->getImage()->image.setHeight(this->getImage()->image.frame.size().height);
	this->getImage()->image.setBytes(this->getImage()->image.frame.channels());
}

Uint32 FrameMaker::size() {

	// array for compressed frame
	std::vector<uchar> compressedFrame;

	// compress preview image with
	// jpeg decoder
	cv::imencode(".jpg", this->getImage()->image.frame,
		compressedFrame);

	ISS_TODO("Need optimization!");

	// return compressed frame size
	return compressedFrame.size();
}

Void FrameMaker::write() {

	// array for compressed frame
	std::vector<uchar> compressedFrame;

	// compress preview image with
	// jpeg decoder
	cv::imencode(".jpg", this->getImage()->image.frame,
		compressedFrame);

	// write compressed frame to
	// package file
	this->getStream()->write(compressedFrame.data(),
		compressedFrame.size());
}