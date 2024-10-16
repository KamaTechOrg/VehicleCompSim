#pragma once
#include <unordered_map>
#include <unordered_set>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <array>
#include <vector>
#include <string>
#include <iostream>

// Custom hash function for std::pair
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};


struct tuple_hash {
    template <class T1, class T2, class T3>
    std::size_t operator() (const std::tuple<T1, T2, T3>& tuple) const {
        std::size_t h1 = std::hash<T1>()(std::get<0>(tuple));
        std::size_t h2 = std::hash<T2>()(std::get<1>(tuple));
        std::size_t h3 = std::hash<T3>()(std::get<2>(tuple));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

enum class ZONE_TYPES
{
    CRITICAL_ZONE_1,
    CRITICAL_ZONE_2,
    TRACKED_ZONE,
    CAR_HOOD,
    GENERAL
};
enum class WarningPriority {
    Critical = 0,
    High = 1,
    Medium = 2,
    Low = 3,
    Safe = 4,
    Unknown = 5
};

struct TrackedObject {
    TrackedObject() :
        distancePrev(-1),
        distanceCurr(-1),
        oldRelativSpeed(-1), 
        zoneTypeIntersect(ZONE_TYPES::GENERAL),
        warningPriority(WarningPriority::Safe)
    {}
    TrackedObject(const std::string& _category, const cv::Rect2d& _rect)
        : 
        category(_category), 
        bboxCurr(_rect),
        distancePrev(-1), 
        distanceCurr(-1),
        oldRelativSpeed(-1), 
        zoneTypeIntersect(ZONE_TYPES::GENERAL), 
        warningPriority(WarningPriority::Safe), 
        warningDescription("") {}
    std::string category;
    cv::Rect2d bboxPrev;
    cv::Rect2d bboxCurr;
    float distancePrev;
    float distanceCurr;
    float oldRelativSpeed;
    ZONE_TYPES zoneTypeIntersect;
    WarningPriority warningPriority;
    std::string warningDescription;
};

struct Warning {
    WarningPriority warningPriority;
    cv::Scalar warningColor;
    std::string warningDescription;
};
//trackedObject.warningPriority = WarningPriority::Critical;
//warnings[int(trackedObject.warningPriority)].warningColor = cv::Scalar(0, 0, 255);
