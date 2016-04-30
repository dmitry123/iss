#include "Image.h"
#include "Pack.h"

#include "md5/md5.hh"

Image::Image() {
	this->packLoader_ = new PackLoader(this);
}

Image::~Image() {
	delete this->packLoader_;
}

Void Image::load(StringC fileName) {

	// read image from file with opencv loader
	this->image.frame = cv::imread(fileName, CV_LOAD_IMAGE_COLOR);

	// check for empty
	if (this->image.frame.empty()) {
		throw Exception("ImageLoader : Unable to load image (%s)", fileName);
	}

	// convert frame from grayscale or
	// rgba to rgb color space
	if (this->image.frame.type() == CV_8UC1) {
		cv::cvtColor(this->image.frame, this->image.frame, cv::COLOR_GRAY2RGB);
	}
	else {
		if (this->image.frame.channels() == 4) {
			cv::cvtColor(this->image.frame, this->image.frame, cv::COLOR_RGBA2RGB);
		}
	}

	// get image's width and height
	this->image.width =
		this->image.frame.size().width;
	this->image.height =
		this->image.frame.size().height;

	// compute image's bytes and depth
	this->image.bytes = this->image.frame.elemSize();
	this->image.depth = this->image.bytes * 8;

	// compute image's size
	this->image.size = this->image.bytes *
		this->image.width *
		this->image.height;

	// open filestream to compute md5 hash
	// summ and save in image's md5 buffer
	strcpy(this->md5, MD5(
		std::ifstream(fileName, std::ios::in | std::ios::binary)
	).hex_digest());
}

Void Image::save(StringC fileName) {

	if (!cv::imwrite(fileName, this->image.frame)) {
		throw Exception("ImageLoader : Unable to save image (%s)", fileName);
	}
}