#include "laneDetction.h"
#include "lane_departure.h"
#include "runLaneDetection.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

std::string videoPath = "lane_vid2.mp4";
std::vector<cv::Point> departure_video_roi_points =
{ cv::Point(200, 690) ,cv::Point(610, 530), cv::Point(1200, 690) };
std::vector<cv::Point> curved_video_roi_points =
{ cv::Point(300, 615), cv::Point(630, 445), cv::Point(1200, 655) };



void display_red_line(cv::Mat& frame, cv::Vec4i& line);
int runLaneDetection();


int runLaneDetection() {
	cv::VideoCapture cap(videoPath);
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
		std::vector<std::vector<int>> lanes = detect_lanes(frame, departure_video_roi_points, true);

		// Check for lane departure
		int lane_depatured = is_lane_departure(frame, lanes, 30);
		if (lane_depatured != 0) {
			putText(frame, "Warning: Lane Departure Detected!", cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0, 0, 255), 8);

			if (lane_depatured == 1) {
				cv::Vec4i lane(lanes[1][0], lanes[1][1], lanes[1][2], lanes[1][3]);
				display_red_line(frame, lane);
			}
			if (lane_depatured == 2) {
				cv::Vec4i lane(lanes[0][0], lanes[0][1], lanes[0][2], lanes[0][3]);
				display_red_line(frame, lane);
			}
		}

		imshow("Lane Departure", frame);
		if (cv::waitKey(1) == 'q') break;
	}

	cap.release();
	cv::destroyAllWindows();
	return 0;
}

void display_red_line(cv::Mat& frame, cv::Vec4i& lane)
{
	// Create a copy of the original frame for the mask
	cv::Mat overlay;
	frame.copyTo(overlay);

	// Draw the red line on the overlay (mask)
	cv::line(overlay, cv::Point(lane[0], lane[1]), cv::Point(lane[2], lane[3]), cv::Scalar(0, 0, 255), 10);

	// Blend the overlay with the original frame using transparency
	double alpha = 0.6;  // Transparency level (0 means fully transparent, 1 means fully opaque)
	cv::addWeighted(overlay, alpha, frame, 1 - alpha, 0, frame);

}
