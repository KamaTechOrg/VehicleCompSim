#include "lane_departure.h"


// Lane departure detection logic
bool is_lane_departure(const cv::Mat& image, const std::vector<std::vector<int>>& lanes, int threshold) {
    int frame_width = image.cols;

    if (lanes.size() != 2) {
        // Not enough lane lines detected to assess lane departure
        return false;
    }

    int left_x2 = lanes[0][2];  // x-coordinate of the right end of the left lane line
    int right_x2 = lanes[1][2]; // x-coordinate of the left end of the right lane line
    int lane_center = (left_x2 + right_x2) / 2;  // Center of the detected lane
    int frame_center = frame_width / 2;  // Center of the frame

    //// Calculate smoothed lane center
    //int smoothed_lane_center = get_smoothed_lane_center(lane_center);

    //// Deviation threshold can now be passed as a parameter
    //int deviation = smoothed_lane_center - frame_center;
    return abs(lane_center - frame_center) > threshold;
}
