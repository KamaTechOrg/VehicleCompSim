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
	static void drawCarsHoodTarpezoid(cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects);
	static void drawZones(cv::Mat& frame);
	static void drawWarningDescription(cv::Mat& frame, const TrackedObject& trackedObject);

	
};
//extern cv::Scalar pink;
//extern cv::Scalar blue;
//extern cv::Scalar green;
//extern cv::Scalar yellow;
//extern cv::Scalar red;
//extern cv::Scalar white;
//extern cv::Scalar black;
//extern cv::Scalar lightBlue;
//extern cv::Scalar purple;
//
//public:
//	static void drawTrackedObjects(cv::Mat& frame, std::unordered_map<int, TrackedObject>& trackedObjects);
//	static void drawPredictedObjects(cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects);
//	static void drawDistance(cv::Mat& frame, std::unordered_map<int, TrackedObject>& trackedObjects);
//	static void drawRelativSpeed(cv::Mat& frame, std::unordered_map<int, TrackedObject>& trackedObjects);
//	static void drawCategory(cv::Mat& frame, std::unordered_map<int, TrackedObject>& trackedObjects);
//	static void drawCategory(cv::Mat& frame, std::unordered_map<int, TrackedObject>& trackedObjects, std::unordered_map<int, TrackedObject>& predictedObjects);
//	static void drawCategory(cv::Mat& frame, std::unordered_map<int, TrackedObject>& trackedObjects, std::unordered_map<int, TrackedObject>& predictedObjects, std::unordered_map<int, TrackedObject>& detectedObjects);
//	static void drawCategory(cv::Mat& frame, std::unordered_map<int, TrackedObject>& trackedObjects, std::unordered_map<int, TrackedObject>& predictedObjects, std::unordered_map<int, TrackedObject>& detectedObjects, std::unordered_map<int, TrackedObject>& lostObjects);
//	static void drawCategory(cv::Mat& frame, std::unordered_map<int, TrackedObject>& trackedObjects, std::unordered_map<int, TrackedObject>& predictedObjects, std::unordered_map<int, TrackedObject>& detectedObjects, std::unordered_map<int, TrackedObject>& lostObjects, std::unordered_map<int, TrackedObject>& newObjects);
//	static void drawCategory(cv::Mat& frame, std::unordered_map<int, TrackedObject>& trackedObjects, std::unordered_map<int, TrackedObject>& predictedObjects, std::unordered_map<int, TrackedObject>& detectedObjects, std::unordered_map<int, TrackedObject>& lostObjects, std::unordered_map<int, TrackedObject>& newObjects, std::unordered_map<int, TrackedObject>& removedObjects);
//	static void drawCategory(cv::Mat& frame, std::unordered_map<int, TrackedObject>& trackedObjects, std::unordered_map<int, TrackedObject>& predictedObjects, std::unordered_map<int, TrackedObject>& detectedObjects, std::unordered_map<int, TrackedObject>& lostObjects, std::unordered_map


