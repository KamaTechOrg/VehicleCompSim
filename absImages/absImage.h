#pragma once
#include <vector>
#include <string>
#include <opencv2/core/types.hpp>

std::vector<cv::Rect> detectChanges(const cv::Mat& img1, const cv::Mat& img2, int threshold = 80, int mergeThreshold = 30, int minDimension = 7);

std::vector<cv::Rect> mergeOverlappingRectangles(const std::vector<cv::Rect>& rects, int threshold);