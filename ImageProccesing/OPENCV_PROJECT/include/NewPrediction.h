#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include "tracking.h"
#include "util.h"
#include "constants.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "constants.h"
#include "Poligon.h"

class NewPrediction
{
public:
	static bool isApproachingDangerZone(const Poligon& tarpezoid, TrackedObject& trackedObject);
	static void updateWarnings(std::unordered_map<int, TrackedObject>& predictedObjects);
    static void matchOverlapBoxes(std::unordered_map<int, TrackedObject>& OldPredictedObjects, std::unordered_map<int, TrackedObject>& NewPredictedObjects);

private:
    static std::optional<cv::Point> findIntersection(const cv::Point& p1, const cv::Point& p2,
        const cv::Point& q1, const cv::Point& q2);
    static std::pair<int, int> calculateDeltas(const cv::Rect& prevRect, const cv::Rect& currRect);
    static std::pair<std::pair<cv::Point, cv::Point>, std::pair<cv::Point, cv::Point>> chooseVectors(const cv::Rect& prevRect, const cv::Rect& currRect, int deltaX, int deltaY);
    static bool adjustVectorForIntersection(
        std::pair<cv::Point, cv::Point>& vector,
        const std::pair<cv::Point, cv::Point>& imageBorder);
    static void adjustVectors(
        std::pair<cv::Point, cv::Point>& firstVector,
        std::pair<cv::Point, cv::Point>& secondVector,
        int deltaX, int deltaY);
    static std::pair<std::pair<cv::Point, cv::Point>, std::pair<cv::Point, cv::Point>> getMovementDirection(const cv::Rect& prevRect, const cv::Rect& currRect);


    static void updateWarningPriorityAndDescription(TrackedObject& trackedObject, ZONE_TYPES zoneType, bool useSpeed = false);
    static void updateWarningPriorityAndDescription(TrackedObject& trackedObject, int mapType);
    static bool checkCriticalZone1(TrackedObject& trackedObject);
    static bool checkCriticalZone2(TrackedObject& trackedObject);
    static void handleTrackedZone(TrackedObject& trackedObject);

    static double calculateIoU(const cv::Rect& rect1, const cv::Rect& rect2);
    static double calculateCentroidDistance(const cv::Rect& r1, const cv::Rect& r2);

    static void findBestOverlap(const TrackedObject& newObject, std::unordered_map<int, TrackedObject>& oldObjects, int& maxAreaId);
    static void findBestCentroidMatch(const TrackedObject& newObject, std::unordered_map<int, TrackedObject>& oldObjects, int& minDistanceId);
    static void updatePredictedObjects(int id, TrackedObject& newObject, std::unordered_map<int, TrackedObject>& oldObjects);
};

