#include "ApproachDangerZone.h"
#include "constants.h"

//=============================================================================
std::unordered_set<int> ApproachDangerZone::getApproachingDangerZoneIDs(const std::unordered_map<int, TrackedObject>& predictedObjects) {
    std::unordered_set<int> approachingObjectsID;
    const std::vector<cv::Point> poligon = { 
        TRAPEZOID::CRITICAL_ZONE::DL, 
        TRAPEZOID::CRITICAL_ZONE::DR,
        TRAPEZOID::CRITICAL_ZONE::UR,
        TRAPEZOID::CRITICAL_ZONE::UL
    };

    for (const auto& [id, predictedObject] : predictedObjects) {
        //is in the torqouise trapezoid? if no, continue?????????????????????????????????????????
        if (predictedObject.category == "car" && predictedObject.oldRelativSpeed >= 0)
            continue;
        if (iApproachingDangerZone(poligon, predictedObject))
            approachingObjectsID.insert(id);
    }

    return approachingObjectsID;
}
//=============================================================================
bool ApproachDangerZone::iApproachingDangerZone(
    const std::vector<cv::Point>& poligon,
    const TrackedObject& trackedObject)
{
    cv::Rect2d nextPositionRect = predictNextPosition(trackedObject);
    return doesRectIntersectPolygon(poligon, nextPositionRect);
}
//=============================================================================
bool ApproachDangerZone::doesRectIntersectPolygon(const std::vector<cv::Point>& poligon, const cv::Rect& rect)
{
    std::vector<cv::Point> rectPolygon;
    rectPolygon.push_back(cv::Point(rect.x, rect.y));
    rectPolygon.push_back(cv::Point(rect.x + rect.width, rect.y));
    rectPolygon.push_back(cv::Point(rect.x + rect.width, rect.y + rect.height));
    rectPolygon.push_back(cv::Point(rect.x, rect.y + rect.height));

    std::vector<cv::Point> intersection;

    if (cv::intersectConvexConvex(rectPolygon, poligon, intersection, true) > 0) {
        return true;
    }
    return false;
}
//=============================================================================
cv::Rect2d ApproachDangerZone::predictNextPosition(const TrackedObject& trackedObject) 
{
    const cv::Rect2d& currentPos = trackedObject.bboxCurr;
    const cv::Rect2d& previousPos = trackedObject.bboxPrev;

    double dx = currentPos.x - previousPos.x;
    double dy = currentPos.y - previousPos.y;

    double dWidth = currentPos.width - previousPos.width;
    double dHeight = currentPos.height - previousPos.height;

    return cv::Rect2d(
        (currentPos.x + dx) > 0 ? currentPos.x + dx : 0,
        (currentPos.y + dy) > 0 ? currentPos.y + dy : 0,
        (currentPos.width + dWidth) > 0 ? currentPos.width + dWidth : 0,
        (currentPos.height + dHeight) > 0 ? currentPos.height + dHeight : 0
    );
}
//=============================================================================