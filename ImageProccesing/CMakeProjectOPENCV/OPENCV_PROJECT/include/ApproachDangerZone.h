#pragma once


#include <opencv2/opencv.hpp>
#include <iostream>
#include "util.h"
//#include "constants.h"

class TrackedObject;

class ApproachDangerZone
{
public:
	static std::unordered_set<int> getApproachingDangerZoneIDs(const std::unordered_map<int, TrackedObject>& predictedObjects);
private:
	static bool iApproachingDangerZone(const std::vector<cv::Point>& poligon, const TrackedObject& trackedObject);
	static bool doesRectIntersectPolygon(const std::vector<cv::Point>& poligon, const cv::Rect& rect);
	static cv::Rect2d predictNextPosition(const TrackedObject& trackedObject);
};

