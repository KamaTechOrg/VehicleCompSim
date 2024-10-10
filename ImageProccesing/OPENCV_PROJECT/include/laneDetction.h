#ifndef LANE_DETECTION_H
#define LANE_DETECTION_H

#include <opencv2/opencv.hpp>
#include <vector>

// Function declarations related to lane detection
std::pair<float, float> polyfit(const std::vector<float>& x, const std::vector<float>& y);
std::vector<int> make_points(const cv::Mat& image, const std::pair<float, float>& line);
std::vector<std::vector<int>> average_slope_intercept(const cv::Mat& image, const std::vector<cv::Vec4i>& lines);
cv::Mat canny(const cv::Mat& img);
cv::Mat display_lines(cv::Mat& img, const std::vector<std::vector<int>>& lines);
cv::Mat region_of_interest(const cv::Mat& img, const std::vector<cv::Point>& points);
std::vector<std::vector<int>> detect_lanes(cv::Mat& image, const std::vector<cv::Point>& points, bool isImgShow = false);

#endif // LANE_DETECTION_H
