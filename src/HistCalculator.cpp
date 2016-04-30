#include "Extension.h"
#include "Pack.h"
#include "Image.h"

namespace cv {
	inline size_t getOptimalDCTSize(size_t N) {
		return 2 * cv::getOptimalDFTSize((N + 1) / 2);
	}
}

Void HistCalculator::process() {

	std::vector<cv::Mat> bgr_planes;

	// sizes for rgb and
	// hsv histogramms
	Sint32 histSize  = 256;
	Sint32 histSize2 = 361;

	// ranges for rgb and hsv
	// histogramms
	Float32 range[]  = { 0, 256 };
	Float32 range2[] = { 0, 361 };

	const Float32* histRange = { range };
	const Float32* histRange2 = { range2 };

	// 3 channels RGB/HSV
	cv::Mat b_hist;
	cv::Mat g_hist;
	cv::Mat r_hist;

	// temporary image matrix
	cv::Mat image;

	// resize image's basic frame to square
	// scale to compute histogramms
	cv::resize(*this->getImage()->image.getFrame(), image, cv::Size(512, 512));

	// split frame into 3 planes (RGB)
	cv::split(image, bgr_planes);

	// calc hist for every plane
	cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, TRUE, FALSE);
	cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, TRUE, FALSE);
	cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, TRUE, FALSE);

	// normalize histogramms
	cv::normalize(b_hist, b_hist, 0, 100, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(g_hist, g_hist, 0, 100, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(r_hist, r_hist, 0, 100, cv::NORM_MINMAX, -1, cv::Mat());

	// get image's hist info structure
	HistInfo* histInfo = &this->getImage()->hist;

	// copy result to image's hist info
	for (Uint32 i = 0; i <= 255; i++) {
		histInfo->red[i] = r_hist.at<Float32>(i);
		histInfo->green[i] = g_hist.at<Float32>(i);
		histInfo->blue[i] = b_hist.at<Float32>(i);
	}

	// now convert image to hsv color mode
	cv::cvtColor(image, image, cv::COLOR_RGB2HSV);

	// split frame into 3 channels (HSV)
	cv::split(image, bgr_planes);

	// calc hist for every channel
	cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), r_hist, 1, &histSize2, &histRange2, TRUE, FALSE);
	cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize2, &histRange2, TRUE, FALSE);
	cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), b_hist, 1, &histSize2, &histRange2, TRUE, FALSE);

	// normalize histogramms [0.0, 100.0]
	cv::normalize(r_hist, r_hist, 0, 100, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(g_hist, g_hist, 0, 100, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(b_hist, b_hist, 0, 100, cv::NORM_MINMAX, -1, cv::Mat());

	// copy result to hist info structure, where
	for (Uint32 i = 0; i <= 360; i++) {
		histInfo->hue[i] = r_hist.at<Float32>(i);
		histInfo->saturation[i] = g_hist.at<Float32>(i);
		histInfo->value[i] = b_hist.at<Float32>(i);
	}
}

Void HistCalculator::read() {

	// get image's hist info
	HistInfo* histInfo = &this->getImage()->hist;

	// jump to out position
	this->getStream()->seek(this->getPosition());

	// read rgb channels from stream
	this->getStream()->read(histInfo->red, sizeof(HistRgb));
	this->getStream()->read(histInfo->green, sizeof(HistRgb));
	this->getStream()->read(histInfo->blue, sizeof(HistRgb));

	// read hsv channels from stream
	this->getStream()->read(histInfo->hue, sizeof(HistHsv));
	this->getStream()->read(histInfo->saturation, sizeof(HistHsv));
	this->getStream()->read(histInfo->value, sizeof(HistHsv));
}

Uint32 HistCalculator::size() {

	// result is size of six histrogramms, where
	// 3 hists is rgb channel and other 3 is hsv channel.
	// rgb color space has range from 0 to 255 while hsv
	// has range from 0 to 360 (don't have to be same
	// in another systems where it might be [0, 1]
	// or [0, 100] or [0,240])
	return
		sizeof(HistRgb) * 3 +
		sizeof(HistHsv) * 3;
}

Void HistCalculator::write() {

	// get pointer to image's hist info
	HistInfo* histInfo = &this->getImage()->hist;

	// write rgb channels to stream
	this->getStream()->write(histInfo->red, sizeof(HistRgb));
	this->getStream()->write(histInfo->green, sizeof(HistRgb));
	this->getStream()->write(histInfo->blue, sizeof(HistRgb));

	// write hsv channels to stream
	this->getStream()->write(histInfo->hue, sizeof(HistHsv));
	this->getStream()->write(histInfo->saturation, sizeof(HistHsv));
	this->getStream()->write(histInfo->value, sizeof(HistHsv));
}