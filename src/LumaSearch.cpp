#include "SearchEngine.h"

Void LumaSearch::process(ImagePtr image) {

	Vector<HashInfo> distanceMap;

	// read image's hist information
	image->getLoader()->getExtension<HistCalculator>()
		->process();

	// save in opencv matrix array
	cv::MatND lumaArraySrc(1, 255, CV_32FC1, image->hist.value);

	for (Buffer b : this->getEngine()->getSourceList()) {

		Image temporaryImage;

		// open stream for temporary image
		temporaryImage.getLoader()
			->open(b);

		// read histogramms from image
		temporaryImage.getLoader()->getExtension<HistCalculator>()
			->read();

		// build opencv matrixes
		cv::MatND lumaArray(1, 255, CV_32FC1, temporaryImage.hist.value);

		// compare histogramms
		Float64 s = cv::compareHist(lumaArraySrc, lumaArray, CV_COMP_CORREL);

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