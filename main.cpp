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


int main() {
	cv::VideoCapture cap("lane_vid2.mp4");
	if (!cap.isOpened()) {
		std::cerr << "Error opening video file" << std::endl;
		return -1;
	}

	while (cap.isOpened()) {
		cv::Mat frame;
		cap >> frame;
		if (frame.empty()) {
			std::cout << "Video has ended or file cannot be read." << std::endl;
			break;
		}

		// Detect lanes and optionally display them on the frame
		std::vector<std::vector<int>> lanes = detect_lanes(frame);

		// Check for lane departure
		if (is_lane_departure(frame, lanes)) {
			putText(frame, "Warning: Lane Departure Detected!", cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 8);
		}

		imshow("Lane Departure", frame);
		if (cv::waitKey(1) == 'q') break;
	}

	cap.release();
	cv::destroyAllWindows();
	return 0;
}