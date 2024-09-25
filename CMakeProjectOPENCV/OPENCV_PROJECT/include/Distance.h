#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <unordered_map>
#include "util.h"
#include "constants.h"


class Distance {
public:
	static float estimateDistanceWithWidth(float objectWidthInPixels, std::string& category);
	static float estimateDistanceWithHeight(float objectHeightInPixels, std::string& category);
    //static void calcDist(const std::vector<std::pair<cv::Rect, std::string>>& detectedObjects, cv::Mat& frame);
	static void calcDist(std::unordered_map<int, TrackedObject>& predictedObjects, cv::Mat& frame);
	static void calcRelativSpeed(std::unordered_map<int, TrackedObject>& predictedObjects, cv::Mat& frame,bool isDetection);
};