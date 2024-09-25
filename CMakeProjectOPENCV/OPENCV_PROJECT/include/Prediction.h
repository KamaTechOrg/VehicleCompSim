#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include "util.h"
#include "constants.h"
#include <unordered_map>
#include <unordered_set>
class Prediction
{
public:
    cv::Rect2d predictNextPosition(const TrackedObject& trackedObject) const;
    std::unordered_map<int, TrackedObject>& getPredictedObjects();
    std::unordered_set<int> analyzeSituation(double frameArea);
    void setCanPredict(bool canPredict);
    bool getCanPredict() const;
    static bool isApproachingDangerZone(const std::vector<cv::Point>& poligon, const TrackedObject& trackedObject);
private:
    bool m_canPredict = false;
    std::unordered_map<int, TrackedObject> m_predictedObjects;

};
