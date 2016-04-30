#include "SearchEngine.h"

// set this flag to 1 if you
// want to render all founded
// faces into output file "faces.jpg"
// ordered by distance and display
// it (for debug)
#define RENDER_RESULT 0

#if RENDER_RESULT

// last x & y positions
static int lastX = 0;
static int lastY = 0;

// result frame
Frame frame;

// move pointer to next line
Void nextLine() {
	lastY += 128;
	lastX = 0;
}

// render frame to buffer
Void displayFrame(Frame& frame, Float64 distance) {

	char t[50];

	// check for buffer's end
	if (lastX + 128 > ::frame.size().width) {
		nextLine();
	}

	// copy frame to result
	frame.copyTo(::frame(cv::Rect(lastX, lastY, frame.size().width, frame.size().height)));

	// convert distance to string
	sprintf(t, "%.4f", distance);

	// put text to result frame
	cv::putText(::frame, t, cv::Point(lastX + 10, lastY + 30),
		cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 255), 1, CV_AA);

	// move x position
	lastX += frame.size().width;
}
#endif

Void FaceSearch::process(ImagePtr image) {

	Vector<HashInfo> distanceMap;

	// read image's hist information
	image->getLoader()->getExtension<FaceDetector>()
		->process();

	if (image->faces.size() != 1) {
		throw Exception("Image (%s) must have one face (%d)", image->md5, image->faces.size());
	}

#if RENDER_RESULT
	// clone current frame (doesn't metter)
	frame = image->image.getFrame()->clone();
	// convert color to grayscale
	cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
	// resize frame to very big size
	cv::resize(frame, frame, cv::Size(1000, 10000));
#endif

	cv::Mat mainFace = image->faces[0];
	cv::Mat grayImage;

	std::map<Uint32, cv::Mat> imgArray;

	Uint32 faceCount = 0;

	// get eigen's face recognizer
	cv::Ptr<cv::FaceRecognizer> faceRecognizer =
		cv::createEigenFaceRecognizer();

	// conver matrix to frayscale
	cv::cvtColor(*image->image.getFrame(), grayImage, cv::COLOR_RGB2GRAY);

	for (Buffer b : this->getEngine()->getSourceList()) {

		Image temporaryImage;

		// open stream for temporary image
		temporaryImage.getLoader()->open(b)
			->getExtension<FaceDetector>()->read();

		std::vector<cv::Mat> faceArray;
		std::vector<Sint32> labelArray;

		for (Frame& f : temporaryImage.faces) {

			// save face and it's label
			faceArray.push_back(f);
			labelArray.push_back(faceCount);

			// save link to label
			imgArray[faceCount++] = f;
		}

#if 0
		if (temporaryImage.faces.size() == 1) {

			// create temporary matrix
			Frame tempMat = temporaryImage.faces[0].clone();

			// set to nothing
			tempMat.setTo(cv::Scalar(0, 0, 0));

			// save face and it's label
			faceArray.push_back(tempMat);
			labelArray.push_back(faceCount);

			// save link to label
			imgArray[faceCount++] = tempMat;
		}
#endif

		Sint32 resultLabel;
		Float64 resultDistance;

		// if we havn't found faces, then
		// goto next picture
		if (!faceArray.size()) {
			continue;
		}

		// train face recognizer and predict
		faceRecognizer->train(faceArray, labelArray);
		faceRecognizer->predict(mainFace, resultLabel, resultDistance);

		if (!resultDistance) {
			continue;
		}

		// save result
		distanceMap.push_back({ b, resultDistance, resultLabel });
	}

	// sort array by distance
	std::sort(distanceMap.begin(), distanceMap.end(), [](const HashInfo& left, const HashInfo& right) -> bool {
		return left.distance < right.distance;
	});

	Uint32 resultCounter = 0;
	Sint8 number[10];

	for (HashInfo& i : distanceMap) {

#if RENDER_RESULT
		// render current face to result frame
		displayFrame(imgArray[i.label], i.distance);
#endif

		// result limit
		if (resultCounter++ == this->getEngine()->getResultCount()) {
			break;
		}

		this->getEngine()->getResultList()
			.push_back(i.path);
	}

#if RENDER_RESULT
	// write croped result to file
	cv::imwrite("faces.jpg", frame(cv::Rect(0, 0, frame.size().width, lastY)));
#endif
}