#ifndef LANE_DEPARTURE_H
#define LANE_DEPARTURE_H

#include <opencv2/opencv.hpp>
#include <vector>

// Function declarations related to lane departure detection
bool is_lane_departure(const cv::Mat& image, const std::vector<std::vector<int>>& lanes, int threshold = 20);

#endif // LANE_DEPARTURE_H
