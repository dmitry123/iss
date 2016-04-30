#include "Extension.h"
#include "Pack.h"
#include "Image.h"

Void PreviewMaker::process() {

	// get image's width & height
	Uint32 imageWidth = this->getImage()->image.getWidth();
	Uint32 imageHeight = this->getImage()->image.getHeight();

	// compute image's aspect ratio
	Float32 aspectRatio = Float32(imageWidth) / Float32(imageHeight);

	// default preview width & height
	Uint32 preWidth = FRAME_WIDTH;
	Uint32 preHeight = Uint32(preWidth / aspectRatio);

	ImageInfo* imageInfo   = &this->getImage()->image;
	ImageInfo* previewInfo = &this->getImage()->preview;

	// temporary matrix
	cv::Mat previewMat;

	// if image width bellow then height
	// then set swap width and height
	if (imageWidth < imageHeight) {
		preHeight = FRAME_HEIGHT;
		preWidth = Uint32(preHeight * aspectRatio);
	}

	// distance between left frame's edge & left preview's edge
	Sint32 diff = std::abs(
		Sint32(preWidth) - Sint32(preHeight)) / 2;

	// resize image to square and save in
	// preview matrix
	cv::resize(imageInfo->frame, previewMat,
		cv::Size(preWidth, preHeight));

	// clone preview mat and push to preview's frame
	previewInfo->frame = previewMat.clone();

	// resize preview's frame to nessesary size
	cv::resize(previewInfo->frame, previewInfo->frame, cv::Size(
		FRAME_WIDTH, FRAME_HEIGHT));
	// set preview's default color as black (#000000)
	previewInfo->frame.setTo(cv::Scalar(0, 0, 0));

	// if image's width above then it's height then
	// copy to preview matrix our image with y offset
	// else copy it with x offset
	if (imageWidth > imageHeight) {
		previewMat.copyTo(previewInfo->frame(cv::Rect(0, diff, preWidth, preHeight)));
	} else {
		previewMat.copyTo(previewInfo->frame(cv::Rect(diff, 0, preWidth, preHeight)));
	}

	// save preview parameters likes
	// width, height, depth, size (auto-compute)
	previewInfo->setWidth(previewInfo->frame.size().width);
	previewInfo->setHeight(previewInfo->frame.size().height);
	previewInfo->setBytes(previewInfo->frame.channels());
}

Void PreviewMaker::read() {

	// array for compressed frame
	std::vector<uchar> compressedFrame(this->getSize());

	// jump to out position
	this->getStream()->seek(this->getPosition());

	// read compressed data from pack
	this->getStream()->read(compressedFrame.data(),
		compressedFrame.size());

	// decode preview data
	this->getImage()->preview.frame = cv::imdecode(
		compressedFrame, cv::IMREAD_COLOR);

	// save preview parameters likes
	// width, height, depth, size (auto-compute)
	this->getImage()->preview.setWidth(this->getImage()->preview.frame.size().width);
	this->getImage()->preview.setHeight(this->getImage()->preview.frame.size().height);
	this->getImage()->preview.setBytes(this->getImage()->preview.frame.channels());
}

Uint32 PreviewMaker::size() {

	// array for compressed frame
	std::vector<uchar> compressedFrame;

	// compress preview image with
	// jpeg decoder
	cv::imencode(".jpg", this->getImage()->preview.frame,
		compressedFrame);

	ISS_TODO("Need optimization!");

	// return compressed frame size
	return compressedFrame.size();
}

Void PreviewMaker::write() {

	// array for compressed frame
	std::vector<uchar> compressedFrame;

	// compress preview image with
	// jpeg decoder
	cv::imencode(".jpg", this->getImage()->preview.frame,
		compressedFrame);

	// write compressed frame to
	// package file
	this->getStream()->write(compressedFrame.data(),
		compressedFrame.size());
}