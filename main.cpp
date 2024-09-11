#include "laneDetction.h"
#include "lane_departure.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	VideoCapture cap("lane_vid2.mp4");
	if (!cap.isOpened()) {
		cerr << "Error opening video file" << endl;
		return -1;
	}

	while (cap.isOpened()) {
		Mat frame;
		cap >> frame;
		if (frame.empty()) {
			cout << "Video has ended or file cannot be read." << endl;
			break;
		}

		// Detect lanes and optionally display them on the frame
		vector<vector<int>> lanes = detect_lanes(frame);

		// Check for lane departure
		if (is_lane_departure(frame, lanes)) {
			putText(frame, "Warning: Lane Departure Detected!", Point(50, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 8);
		}

		imshow("Lane Departure", frame);
		if (waitKey(1) == 'q') break;
	}

	cap.release();
	destroyAllWindows();
	return 0;
}