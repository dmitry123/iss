#include "SearchEngine.h"

Void ColorSearch::process(ImagePtr image) {

	Vector<HashInfo> distanceMap;

	// read image's hist information
	image->getLoader()->getExtension<HistCalculator>()
		->process();

	// save in opencv matrix array
	cv::MatND redArraySrc(1, 255, CV_32FC1, image->hist.red);
	cv::MatND greenArraySrc(1, 255, CV_32FC1, image->hist.green);
	cv::MatND blueArraySrc(1, 255, CV_32FC1, image->hist.blue);

	for (Buffer b : this->getEngine()->getSourceList()) {

		Image temporaryImage;

		// open stream for temporary image
		temporaryImage.getLoader()
			->open(b);

		// read histogramms from image
		temporaryImage.getLoader()->getExtension<HistCalculator>()
			->read();

		// build opencv matrixes
		cv::MatND redArray(1, 255, CV_32FC1, temporaryImage.hist.red);
		cv::MatND greenArray(1, 255, CV_32FC1, temporaryImage.hist.green);
		cv::MatND blueArray(1, 255, CV_32FC1, temporaryImage.hist.blue);

		// compare histogramms
		Float64 r2 = cv::compareHist(redArraySrc, redArray, CV_COMP_CORREL);
		Float64 g2 = cv::compareHist(greenArraySrc, greenArray, CV_COMP_CORREL);
		Float64 b2 = cv::compareHist(blueArraySrc, blueArray, CV_COMP_CORREL);

		// euclid's metric
		Float64 s = std::sqrt(
			std::pow(r2, 2) +
			std::pow(g2, 2) +
			std::pow(b2, 2));

		distanceMap.push_back({ b, s });
	}

	// sort array by distance
	std::sort(distanceMap.begin(), distanceMap.end(), [](const HashInfo& left, const HashInfo& right) -> bool {
		return left.distance > right.distance;
	});

	Uint32 resultCounter = 0;

	for (HashInfo& i : distanceMap) {

		// result limit
		if (resultCounter++ == this->getEngine()->getResultCount()) {
			break;
		}

		// save result
		this->getEngine()->getResultList().push_back(i.path);
	}
}