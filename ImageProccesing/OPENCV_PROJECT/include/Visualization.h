#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <unordered_map>
#include "util.h"
#include "constants.h"

class Visualization
{
public:
	static void drawPredictedObjects(cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects);
	static void drawDistance(cv::Mat& frame, cv::Rect& rect, TrackedObject trackedObject);
	static void drawRelativeSpeedWithDistances(cv::Mat& frame, const TrackedObject& predictedObjects);
	static void drawRelativeSpeed(cv::Mat& frame, const TrackedObject& predictedObject, float relativeSpeed);
	static void drawCategory(cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects);
	static void drawZones(cv::Mat& frame);
	static void drawWarningDescription(cv::Mat& frame, const TrackedObject& trackedObject);

	static void setCurrentVideoPoligons(const std::string& videoName);
private:
	static std::vector<std::pair<Poligon, ZONE_TYPES>>* m_zonesPoligons;
};

