#ifdef _DEBUG
// the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world454d.lib")
#else
// the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world454.lib")
#endif

#include "laneDetction.h"
#include <opencv2/opencv.hpp>
#include <iostream>

std::string videoPath = "dashcam4.mp4";

std::vector<cv::Point> departure_video_roi_points =
{ cv::Point(200, 690) ,cv::Point(610, 530), cv::Point(1200, 690) };
//std::vector<cv::Point> curved_video_roi_points =
//{ cv::Point(300, 615), cv::Point(630, 445), cv::Point(1200, 655) };

std::vector<cv::Point> curved_video_roi_points =
{ cv::Point(300, 690), cv::Point(630, 480), cv::Point(1100, 690) };

float calculateSlope(const std::vector<int>& line) {
	if (line.size() != 4) {
		throw std::invalid_argument("The vector must contain 4 values: x1, y1, x2, y2.");
	}

	float x1 = line[0], y1 = line[1];
	float x2 = line[2], y2 = line[3];

	// Checking if the points are on the same vertical line (division by zero case)
	if (x2 - x1 == 0) {
		return 0;
		throw std::invalid_argument("The points create a vertical line, so the slope is undefined.");
	}

	// Calculating the slope
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
		std::vector<std::vector<int>> lanes = detect_lanes(frame, curved_video_roi_points, true);

		if (lanes.size() != 2)
			continue;

		//std::cout << "frame number: " << i++ << " slope = " << calculateSlope(lanes[1]) << std::endl;

		float left_slope = calculateSlope(lanes[0]);
		float right_slope = calculateSlope(lanes[1]);
		std::cout << "average slope: " << left_slope + right_slope << std::endl;

		std::string left_str = std::to_string(left_slope);
		std::string right_str = std::to_string(right_slope);
		putText(frame, "left: " + left_str + " right: " + right_str, cv::Point(50, 690), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
		//if (abs(left_slope + right_slope) > 0.35 && is_lane_departure(frame, lanes) == 0)
		if (abs(left_slope + right_slope) > 0.25)
		{
			if (left_slope + right_slope > 0)
				putText(frame, "Warning: Right curve in front of you", cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
			else
				putText(frame, "Warning: Left curve in front of you", cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
		}

		imshow("Lane Departure", frame);
		if (cv::waitKey(1) == 'q') break;
	}

	cap.release();
	cv::destroyAllWindows();
	return 0;
}
