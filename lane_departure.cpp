#include "lane_departure.h"


// Lane departure detection logic
// returns 1 for right departure, 2 for left departure, and 0 for not departure
int is_lane_departure(const cv::Mat& image, const std::vector<std::vector<int>>& lanes, int threshold) {
    int frame_width = image.cols;

    if (lanes.size() != 2) {
        // Not enough lane lines detected to assess lane departure
        return 0;
    }

    int left_x2 = lanes[0][2];  // x-coordinate of the right end of the left lane line
    int right_x2 = lanes[1][2]; // x-coordinate of the left end of the right lane line
    int lane_center = (left_x2 + right_x2) / 2;  // Center of the detected lane
    int frame_center = frame_width / 2;  // Center of the frame

    if (lane_center - frame_center > threshold) {
        return 1;
    }
    if (lane_center - frame_center < 0 - threshold)
        return 2;
    return 0;
}
