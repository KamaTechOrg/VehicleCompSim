#ifdef _DEBUG
// the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world454d.lib")
#else
// the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world454.lib")
#endif

#include "laneDetction.h"
#include "lane_departure.h"
#include <opencv2/opencv.hpp>
#include <iostream>

std::string videoPath = "lane_vid2.mp4"; 

float calculateSlope(const std::vector<int>& line) {
	if (line.size() != 4) {
		throw std::invalid_argument("הווקטור חייב להכיל 4 ערכים: x1, y1, x2, y2.");
	}

	float x1 = line[0], y1 = line[1];
	float x2 = line[2], y2 = line[3];

	// בדיקה שהנקודות לא נמצאות על אותו קו אנכי (מקרה של חלוקה באפס)
	if (x2 - x1 == 0) {
		throw std::invalid_argument("הנקודות יוצרות קו אנכי ולכן השיפוע אינו מוגדר.");
	}

	// חישוב השיפוע
	return (y2 - y1) / (x2 - x1);
}

int main() {
	cv::VideoCapture cap(videoPath);
	if (!cap.isOpened()) {
		std::cerr << "Error opening video file" << std::endl;
		return -1;
	}
	int i = 0;
	while (cap.isOpened()) {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			std::cout << "Video has ended or file cannot be read." << std::endl;
			break;
		}

		// Detect lanes and optionally display them on the frame
		std::vector<std::vector<int>> lanes = detect_lanes(frame, true);

		std::cout << "frame number: " << i++ << " slote = " << calculateSlope(lanes[0]) << std::endl;
	}

	cap.release();
	cv::destroyAllWindows();
	return 0;
}