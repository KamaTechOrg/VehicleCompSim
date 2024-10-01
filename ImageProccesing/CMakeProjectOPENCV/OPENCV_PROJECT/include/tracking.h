#pragma once
#include "constants.h"
#include "util.h"
#include <unordered_map>
#include <unordered_set>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

//=============================================================================
class TrackerManager {
public:
    void initializeTrackers(const cv::Mat& frame, std::vector<std::pair<cv::Rect, std::string>>& detectedResult, std::unordered_map<int, TrackedObject>& predictedObjects);
    void updateTrackers(const cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects);

private:
    std::unordered_map<int, cv::Ptr<cv::Tracker>> trackers;
};
