#include "Extension.h"
#include "Pack.h"
#include "Image.h"

// set this flag to 0, if you
// want to compare images with
// simple hash
#define PERCEPTUAL_HASH 1

namespace cv {
	inline size_t getOptimalDCTSize(size_t N) {
		return 2 * cv::getOptimalDFTSize((N + 1) / 2);
	}
}

Void HashGenerator::process() {

	// temporary image frame
	cv::Mat imageResult;

	// clone image's basic frame
	imageResult = this->getImage()->image.getFrame()->clone();

	// convert frame to gray scale
	cv::cvtColor(imageResult, imageResult, cv::COLOR_RGB2GRAY);

#if PERCEPTUAL_HASH
	// apply median blur
	cv::medianBlur(imageResult, imageResult, 7);
#endif

	// convert frame's pixels to float (need for DCT)
	imageResult.convertTo(imageResult, CV_32FC1);

#if PERCEPTUAL_HASH
	// resize image to 32x32 for DCT
	cv::resize(imageResult, imageResult, cv::Size(32, 32));

	// get optimal DCT size (it has some predictions
	// for optiomal size and will be crashed with wrong)
	Uint32 m = cv::getOptimalDCTSize(imageResult.rows);
	Uint32 n = cv::getOptimalDCTSize(imageResult.cols);

	// create image's borders with optiomal size
	cv::copyMakeBorder(imageResult, imageResult, 0,
		m - imageResult.rows, 0,
		n - imageResult.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	try {
		// apply discrete cosine transform
		cv::dct(imageResult, imageResult);
	}
	catch (...) {
		// we've got optimal size, and cv::dct
		// won't throw an exception, anyway i'll
		// left here try block to avoid app terminate
		// by only one image
	}

	// after dct we must get left top 64 bytes (8x8 frame)
	// from temporary frame (i'll write more information in docs)
	imageResult(cv::Rect(0, 0, 8, 8)).copyTo(imageResult);

#else
	// resize image to 8x8 to remove high levels
	cv::resize(imageResult, imageResult, cv::Size(8, 8));
#endif

	Uint32 avgMean = 0;

	// compute average value for frame
	for (Uint32 i = 0, y = 0; y < 8; y++) {
		for (Uint32 x = 0; x < 8; x++) {
			avgMean += Uint32(imageResult.at<Float32>(cv::Point(x, y)));
		}
	}
	avgMean /= 64;

	Uint64 hash = 0;

	// build image's hash
	for (Uint32 i = 0, y = 0; y < 8; y++) {
		for (Uint32 x = 0; x < 8; x++) {
			if (imageResult.at<Float32>(cv::Point(x, y)) > avgMean) {
				hash |= 1i64 << i;
			}
			++i;
		}
	}

	// save hash
	this->getImage()->hash = hash;
}

Void HashGenerator::read() {

	// jump to out position
	this->getStream()->seek(this->getPosition());

	// read image's perceptual hash from stream
	this->getStream()->read(&this->getImage()->hash, sizeof(this->getImage()->hash));
}

Uint32 HashGenerator::size() {

	// return image's perceptual hash size (8 bytes)
	return sizeof(this->getImage()->hash);
}

Void HashGenerator::write() {

	// write image's perceptual hash to stream
	this->getStream()->write(&this->getImage()->hash, sizeof(this->getImage()->hash));
}