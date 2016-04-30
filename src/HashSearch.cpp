#include "SearchEngine.h"

Void HashSearch::process(ImagePtr image) {

	Vector<HashInfo> distanceMap;

	// read image's hist information
	image->getLoader()->getExtension<HashGenerator>()
		->process();

	for (Buffer b : this->getEngine()->getSourceList()) {

		Image temporaryImage;

		// open stream for temporary image
		// and read histogramms from image
		temporaryImage.getLoader()->open(b)->getExtension<HashGenerator>()
			->read();

		distanceMap.push_back({ b,
			image->hash.distance(temporaryImage.hash)
		});
	}

	// sort array by distance
	std::sort(distanceMap.begin(), distanceMap.end(), [](const HashInfo& left, const HashInfo& right) -> bool {
		return left.distance < right.distance;
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