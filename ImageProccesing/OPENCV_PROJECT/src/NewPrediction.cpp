#include "NewPrediction.h"
#include <optional>
//=================================================================================================
double NewPrediction::calculateCentroidDistance(const cv::Rect& r1, const cv::Rect& r2)
{
    cv::Point2f c1(r1.x + r1.width / 2.0f, r1.y + r1.height / 2.0f);
    cv::Point2f c2(r2.x + r2.width / 2.0f, r2.y + r2.height / 2.0f);
    return cv::norm(c1 - c2); // Euclidean distance between centroids
}
//=============================================================================
double NewPrediction::calculateIoU(const cv::Rect& rect1, const cv::Rect& rect2) {
    cv::Rect intersection = rect1 & rect2;
    double intersectionArea = static_cast<double>(intersection.area());
    double unionArea = static_cast<double>(rect1.area() + rect2.area() - intersectionArea);
    return (unionArea > 0) ? intersectionArea / unionArea : 0.0;
}
//=============================================================================
void NewPrediction::findBestOverlap(const TrackedObject& newObject, std::unordered_map<int, TrackedObject>& oldObjects, int& maxAreaId) {
    double maxArea = 0;
    for (auto& [id, oldObject] : oldObjects) {
        if (newObject.category != oldObject.category) continue;
        double areaIntersection = calculateIoU(newObject.bboxCurr, oldObject.bboxCurr);
        if (areaIntersection > MIN_INTERSECTION_AREA && areaIntersection > maxArea) {
            maxArea = int(areaIntersection);
            maxAreaId = id;
        }
    }
}
//=============================================================================
void NewPrediction::findBestCentroidMatch(const TrackedObject& newObject, std::unordered_map<int, TrackedObject>& oldObjects, int& minDistanceId) {
    int minDistance = 1000000;
    for (auto& [id, oldObject] : oldObjects) {
        if (newObject.category != oldObject.category) continue;
        double distance = calculateCentroidDistance(newObject.bboxCurr, oldObject.bboxCurr);
        if (distance < CENTER_DISTANCE_THRESHOLD && distance < minDistance) {
            minDistance = int(distance);
            minDistanceId = id;
        }
    }
}
//=============================================================================
void NewPrediction::updatePredictedObjects(int id, TrackedObject& newObject, std::unordered_map<int, TrackedObject>& oldObjects)
{
    newObject.distancePrev = oldObjects[id].distanceCurr;
    oldObjects.erase(id);
}
//=============================================================================
void NewPrediction::matchOverlapBoxes(std::unordered_map<int, TrackedObject>& OldPredictedObjects, std::unordered_map<int, TrackedObject>& NewPredictedObjects) {
    for (auto& [id, newPredictedObject] : NewPredictedObjects) {
        int maxAreaId = -1;
        findBestOverlap(newPredictedObject, OldPredictedObjects, maxAreaId);

        if (maxAreaId != -1) {
            updatePredictedObjects(maxAreaId, newPredictedObject, OldPredictedObjects);
        }
        else {
            int minDistanceId = -1;
            findBestCentroidMatch(newPredictedObject, OldPredictedObjects, minDistanceId);

            if (minDistanceId != -1)
                updatePredictedObjects(minDistanceId, newPredictedObject, OldPredictedObjects);
        }
    }
}
//=================================================================================================
std::optional<cv::Point> NewPrediction::findIntersection(const cv::Point& p1, const cv::Point& p2,
    const cv::Point& q1, const cv::Point& q2) {

    int a1 = p2.y - p1.y; // y2 - y1
    int b1 = p1.x - p2.x; // x1 - x2
    int c1 = a1 * p1.x + b1 * p1.y;

    int a2 = q2.y - q1.y; // y2 - y1
    int b2 = q1.x - q2.x; // x1 - x2
    int c2 = a2 * q1.x + b2 * q1.y;

    int determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) {
        return std::nullopt;
    }
    else {
        auto& cols = IMAGE_BORDERS.first;
        auto& rows = IMAGE_BORDERS.second;

        int x = (b2 * c1 - b1 * c2) / determinant;
        int y = (a1 * c2 - a2 * c1) / determinant;

        if (x < 0 || x > cols || y < 0 || y > rows) {
            return std::nullopt;
        }

        return cv::Point(x, y);
    }
}
//=================================================================================================
/**
* @brief Calculate Delta Values:
* Calculates the difference in position between the previous and current rectangles.
*/
std::pair<int, int> NewPrediction::calculateDeltas(const cv::Rect& prevRect, const cv::Rect& currRect) {
    int deltaX = currRect.x - prevRect.x;
    int deltaY = currRect.y - prevRect.y;
    return std::make_pair(deltaX, deltaY);
}
//=================================================================================================
/**
* @brief Choose Vectors Based on Delta:
* Selects the appropriate vectors for comparison based on movement direction.
*/
std::pair<std::pair<cv::Point, cv::Point>, std::pair<cv::Point, cv::Point>> NewPrediction::chooseVectors(const cv::Rect& prevRect, const cv::Rect& currRect, int deltaX, int deltaY) {
    std::pair<cv::Point, cv::Point> firstVector;
    std::pair<cv::Point, cv::Point> secondVector;

    cv::Point prevTL = prevRect.tl();
    cv::Point currTL = currRect.tl();
    cv::Point prevTR = cv::Point(prevRect.x + prevRect.width, prevRect.y);
    cv::Point currTR = cv::Point(currRect.x + currRect.width, currRect.y);
    cv::Point prevBL = cv::Point(prevRect.x, prevRect.y + prevRect.height);
    cv::Point currBL = cv::Point(currRect.x, currRect.y + currRect.height);
    cv::Point prevBR = prevRect.br();
    cv::Point currBR = currRect.br();

    if ((deltaX > 0 && deltaY > 0) || (deltaX < 0 && deltaY < 0)) {
        firstVector = std::make_pair(prevTR, currTR);
        secondVector = std::make_pair(prevBL, currBL);
    }
    else {
        firstVector = std::make_pair(prevTL, currTL);
        secondVector = std::make_pair(prevBR, currBR);
    }

    return std::make_pair(firstVector, secondVector);
}
//=================================================================================================
/**
* @brief Adjust Vector Based on Border Intersections:
* Adjusts the movement vectors based on intersections with the image borders.
*/
bool NewPrediction::adjustVectorForIntersection(std::pair<cv::Point, cv::Point>& vector, const std::pair<cv::Point, cv::Point>& imageBorder) {

    auto intersection = findIntersection(vector.first, vector.second, imageBorder.first, imageBorder.second);
    bool isIntersect = intersection.has_value();
    if (isIntersect) {
        vector.second = intersection.value();
    }

    return isIntersect;
}

//=================================================================================================
/**
* @brief Adjust Both Vectors Based on Movement:
* Handles the adjustment of both vectors depending on the movement direction.
*/
void NewPrediction::adjustVectors(
    std::pair<cv::Point, cv::Point>& firstVector,
    std::pair<cv::Point, cv::Point>& secondVector,
    int deltaX, int deltaY) {

    std::pair<cv::Point, cv::Point> imageUpperBorder = std::make_pair(cv::Point(0, 0), cv::Point(IMAGE_BORDERS.first, 0));
    std::pair<cv::Point, cv::Point> imageLowerBorder = std::make_pair(cv::Point(0, IMAGE_BORDERS.second), cv::Point(IMAGE_BORDERS.first, IMAGE_BORDERS.second));
    std::pair<cv::Point, cv::Point> imageLeftBorder = std::make_pair(cv::Point(0, 0), cv::Point(0, IMAGE_BORDERS.second));
    std::pair<cv::Point, cv::Point> imageRightBorder = std::make_pair(cv::Point(IMAGE_BORDERS.first, 0), cv::Point(IMAGE_BORDERS.first, IMAGE_BORDERS.second));

    if (deltaY == 0)
    {
        if (deltaX > 0)
        {
            adjustVectorForIntersection(firstVector, imageLeftBorder);
            adjustVectorForIntersection(secondVector, imageLeftBorder);
        }
        else if (deltaX < 0)
        {
            adjustVectorForIntersection(firstVector, imageRightBorder);
            adjustVectorForIntersection(secondVector, imageRightBorder);
        }
        return;
    }

    bool isFirstIntersect, isSecondIntersect;
    if (deltaY > 0)
    {
        isFirstIntersect = adjustVectorForIntersection(firstVector, imageLowerBorder);
        isSecondIntersect = adjustVectorForIntersection(secondVector, imageLowerBorder);
    }
    else//deltaY < 0
    {
        isFirstIntersect = adjustVectorForIntersection(firstVector, imageUpperBorder);
        isSecondIntersect = adjustVectorForIntersection(secondVector, imageUpperBorder);
    }

    if (deltaX > 0)
    {
        if (!isFirstIntersect)
            adjustVectorForIntersection(firstVector, imageRightBorder);
        if (!isSecondIntersect)
            adjustVectorForIntersection(secondVector, imageRightBorder);
    }
    else if (deltaX < 0)
    {
        if (!isFirstIntersect)
            adjustVectorForIntersection(firstVector, imageLeftBorder);
        if (!isSecondIntersect)
            adjustVectorForIntersection(secondVector, imageLeftBorder);
    }
}
//=================================================================================================
/**
* @brief Main getMovementDirection Function:
* Main function becomes a simple orchestration of the smaller helper functions.
*/
std::pair<std::pair<cv::Point, cv::Point>, std::pair<cv::Point, cv::Point>> NewPrediction::getMovementDirection(const cv::Rect& prevRect, const cv::Rect& currRect) {
    auto [deltaX, deltaY] = calculateDeltas(prevRect, currRect);
    auto [firstVector, secondVector] = chooseVectors(prevRect, currRect, deltaX, deltaY);

    adjustVectors(firstVector, secondVector, deltaX, deltaY);

    return std::make_pair(firstVector, secondVector);
}
//=================================================================================================
//void drawMovementVectorsAndRect(
//    cv::Mat& image,
//    const cv::Rect& currRect,
//    const std::pair<std::pair<cv::Point, cv::Point>, std::pair<cv::Point, cv::Point>>& movementVectors
//){
//    cv::line(image, movementVectors.first.first, movementVectors.first.second, cv::Scalar(0, 255, 0), 2); 
//    cv::line(image, movementVectors.second.first, movementVectors.second.second, cv::Scalar(0, 255, 0), 2); 
//
//    cv::rectangle(image, currRect, cv::Scalar(255, 0, 0), 2);//blue
//}
//=================================================================================================
bool NewPrediction::isApproachingDangerZone(const Poligon& poligon, TrackedObject& trackedObject){
    auto movementVectors = getMovementDirection(trackedObject.bboxPrev, trackedObject.bboxCurr);
    if(poligon.doesLineIntersectPolygon(movementVectors.first) || poligon.doesLineIntersectPolygon(movementVectors.second))
		return true;
    return false;
}
//=================================================================================================
// Helper function to update trackedObject's warning based on category and zone type
void NewPrediction::updateWarningPriorityAndDescription(TrackedObject& trackedObject, int mapType) {
    auto zoneTypeAndCategoryPair = std::make_pair(trackedObject.zoneTypeIntersect, trackedObject.category);

    auto warningPriorityDescriptionIt = PRIORITY_AND_DESCRIPTION[mapType]->find(zoneTypeAndCategoryPair);
    if (warningPriorityDescriptionIt != ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.end()) {
        trackedObject.warningPriority = warningPriorityDescriptionIt->second.first;
        trackedObject.warningDescription = warningPriorityDescriptionIt->second.second;
    }
}
//=================================================================================================
void NewPrediction::updateWarningPriorityAndDescription(TrackedObject& trackedObject, ZONE_TYPES zoneType, bool useSpeed) {
    if (useSpeed) {
        bool isMovingSafely = trackedObject.oldRelativSpeed >= 0;
        auto speedBasedKey = std::make_tuple(zoneType, trackedObject.category, isMovingSafely);

        auto warningPriorityDescriptionIt = SPEED_BASED_ZONE_PRIORITY_DESCRIPTION_MAP.find(speedBasedKey);
        if (SPEED_BASED_ZONE_PRIORITY_DESCRIPTION_MAP.find(speedBasedKey) != SPEED_BASED_ZONE_PRIORITY_DESCRIPTION_MAP.end()) {
            trackedObject.warningPriority = warningPriorityDescriptionIt->second.first;
            trackedObject.warningDescription = warningPriorityDescriptionIt->second.second;
        }
    }
    else {
        auto zoneTypeAndCategoryPair = std::make_pair(zoneType, trackedObject.category);
        auto warningPriorityDescriptionIt = ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.find(zoneTypeAndCategoryPair);
        if (warningPriorityDescriptionIt != ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.end()) {
            trackedObject.warningPriority = warningPriorityDescriptionIt->second.first;
            trackedObject.warningDescription = warningPriorityDescriptionIt->second.second;
        }
    }
}
//=================================================================================================
// Check if the tracked object intersects with CRITICAL_ZONE_1
bool NewPrediction::checkCriticalZone1(TrackedObject& trackedObject) {
    if (ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_1)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {
        trackedObject.zoneTypeIntersect = ZONE_TYPES::CRITICAL_ZONE_1;
        if constexpr (RUN_VERSION_1_OF_MAPS)
            updateWarningPriorityAndDescription(trackedObject, ZONE_TYPES::CRITICAL_ZONE_1);
        else
            updateWarningPriorityAndDescription(trackedObject, GENERAL);
        return true;
    }
    return false;
}
//=================================================================================================
// Check if the tracked object intersects with CRITICAL_ZONE_2
bool NewPrediction::checkCriticalZone2(TrackedObject& trackedObject) {
    if (ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_2)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {
        trackedObject.zoneTypeIntersect = ZONE_TYPES::CRITICAL_ZONE_2;

        if (trackedObject.category != OBJECT_CATEGORY::PERSON) {// For vehicles, consider relative speed => if positive speed: safe/negative speed: high
            if constexpr (RUN_VERSION_1_OF_MAPS)
                updateWarningPriorityAndDescription(trackedObject, ZONE_TYPES::CRITICAL_ZONE_2, true);
            else
                updateWarningPriorityAndDescription(trackedObject, IN_ZONE_2_POSITIVE_SPEED);
        }
        else {//if person...High
            if constexpr (RUN_VERSION_1_OF_MAPS)
                updateWarningPriorityAndDescription(trackedObject, ZONE_TYPES::CRITICAL_ZONE_2);
            else
                updateWarningPriorityAndDescription(trackedObject, GENERAL);
        }
        return true;
    }
    return false;
}
//=================================================================================================
// Handle objects that are in the TRACKED_ZONE
void NewPrediction::handleTrackedZone(TrackedObject& trackedObject) {
    trackedObject.zoneTypeIntersect = ZONE_TYPES::TRACKED_ZONE;//is in the TRACKED_ZONE color

    if (isApproachingDangerZone(ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_1)].first, trackedObject)) {
        if constexpr (RUN_VERSION_1_OF_MAPS)
        {
            trackedObject.warningPriority = WarningPriority::High;
            trackedObject.warningDescription = "Object is moving to CRITICAL_ZONE_1";
        }
        else
            updateWarningPriorityAndDescription(trackedObject, MOVING_TO_ZONE_1);
    }
    else if (isApproachingDangerZone(ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_2)].first, trackedObject)) {
        if (trackedObject.category == OBJECT_CATEGORY::PERSON) {//if person: ..High
            if constexpr (RUN_VERSION_1_OF_MAPS)
            {
                trackedObject.warningPriority = WarningPriority::High;
                trackedObject.warningDescription = "Person is moving to CRITICAL_ZONE_2";
            }
            else
                updateWarningPriorityAndDescription(trackedObject, MOVING_TO_ZONE_2_NEGATIVE_SPEED);
        }
        else {//if car: ..positive speed = WarningPriority::Low/negative speed = WarningPriority::Medium
            if constexpr (RUN_VERSION_1_OF_MAPS)
                updateWarningPriorityAndDescription(trackedObject, ZONE_TYPES::TRACKED_ZONE, true);
            else
                updateWarningPriorityAndDescription(trackedObject, GENERAL);
        }
    }
    else {
        if constexpr (RUN_VERSION_1_OF_MAPS)
            updateWarningPriorityAndDescription(trackedObject, ZONE_TYPES::TRACKED_ZONE);
        else
            updateWarningPriorityAndDescription(trackedObject, MOVING_TO_ZONE_2_POSITIVE_SPEED);
    }
}
//=================================================================================================
// Main function to update warnings for all predicted objects
void NewPrediction::updateWarnings(std::unordered_map<int, TrackedObject>& predictedObjects) {
    for (auto& [id, trackedObject] : predictedObjects) {
        if (trackedObject.category == OBJECT_CATEGORY::TRAFFIC_LIGHT) continue;

        if (checkCriticalZone1(trackedObject)) continue;//CRITICAL_ZONE_1
        if (checkCriticalZone2(trackedObject)) continue;//CRITICAL_ZONE_2: if person...critical./if car : positive speed: safe/negative speed: high

        if (ZONES_POLIGON[int(ZONE_TYPES::TRACKED_ZONE)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {//TRACKED_ZONE
            handleTrackedZone(trackedObject);
        }
        else {
            trackedObject.zoneTypeIntersect = ZONE_TYPES::GENERAL;
            if constexpr (RUN_VERSION_1_OF_MAPS)
            {
                trackedObject.warningPriority = WarningPriority::Safe;
                trackedObject.warningDescription = "";
            }
            else
            {
                updateWarningPriorityAndDescription(trackedObject, GENERAL);
            }
        }
    }
}
//=================================================================================================
//void NewPrediction::updateWarnings(std::unordered_map<int, TrackedObject>& predictedObjects)
//{
//    for (auto& [id, trackedObject] : predictedObjects) {
//        if (trackedObject.category == OBJECT_CATEGORY::TRAFFIC_LIGHT)
//            continue;
//        //CRITICAL_ZONE_1
//        else if (ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_1)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {
//            trackedObject.zoneTypeIntersect = ZONE_TYPES::CRITICAL_ZONE_1;
//            auto zoneTypeAndCatgoryPair = std::make_pair(ZONE_TYPES::CRITICAL_ZONE_1, trackedObject.category);
//            if (ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.find(zoneTypeAndCatgoryPair) != ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.end()) {
//                const auto& warningPriority_and_description = ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.at(zoneTypeAndCatgoryPair);
//                trackedObject.warningPriority = warningPriority_and_description.first;
//                trackedObject.warningDescription = warningPriority_and_description.second;
//            }
//            continue;
//        }
//        //CRITICAL_ZONE_2:
//        //if person...critical./if car : positive speed: safe/negative speed: high
//        else if (ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_2)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {
//            trackedObject.zoneTypeIntersect = ZONE_TYPES::CRITICAL_ZONE_2;
//            auto zoneTypeAndCatgoryPair = std::make_pair(ZONE_TYPES::CRITICAL_ZONE_2, trackedObject.category);
//            //if person...critical
//            if (zoneTypeAndCatgoryPair.second == OBJECT_CATEGORY::PERSON && ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.find(zoneTypeAndCatgoryPair) != ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.end()) {
//                const auto& warningPriority_and_description = ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.at(zoneTypeAndCatgoryPair);
//                trackedObject.warningPriority = warningPriority_and_description.first;
//                trackedObject.warningDescription = warningPriority_and_description.second;
//            }
//            //if car : positive speed: safe/negative speed: high
//            else {
//                auto& distance = trackedObject.oldRelativSpeed;
//                if (distance >= 0) {
//                    trackedObject.warningPriority = WarningPriority::Safe;
//                    trackedObject.warningDescription = "Car in CRITICAL_ZONE_2";
//                }
//                else {
//                    trackedObject.warningPriority = WarningPriority::High;
//                    trackedObject.warningDescription = "Veichle is slowing down in CRITICAL_ZONE_2";
//                }
//            }
//            continue;
//        }
//        //TRACKED_ZONE
//        else if (ZONES_POLIGON[int(ZONE_TYPES::TRACKED_ZONE)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {
//            //color is in the 	TRACKED_ZONE color
//            trackedObject.zoneTypeIntersect = ZONE_TYPES::TRACKED_ZONE;
//            if (isApproachingDangerZone(ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_1)].first, trackedObject)) {
//                trackedObject.warningPriority = WarningPriority::High;
//                trackedObject.warningDescription = "object is move to CRITICAL_ZONE_1";
//                continue;
//            }
//            if (isApproachingDangerZone(ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_2)].first, trackedObject)) {
//                //if person: ..High
//                if (trackedObject.category == OBJECT_CATEGORY::PERSON) {
//                    trackedObject.warningPriority = WarningPriority::High;
//                    trackedObject.warningDescription = "person is move to CRITICAL_ZONE_2";
//                }
//                //if car: ..meduim
//                else {
//                    auto& distance = trackedObject.oldRelativSpeed;
//                    if (distance >= 0) {
//                        trackedObject.warningPriority = WarningPriority::Low;
//                        trackedObject.warningDescription = "Car is moving to CRITICAL_ZONE_2";
//                    }
//                    else {
//                        trackedObject.warningPriority = WarningPriority::Medium;
//                        trackedObject.warningDescription = "object is slowing and moving to CRITICAL_ZONE_2";
//                    }
//                }
//                continue;
//            }
//            else {//object is in TRACKED_ZONE but is not moving into CRITICAL_ZONE_1 or CRITICAL_ZONE_2
//                trackedObject.warningPriority = WarningPriority::Low;
//                trackedObject.warningDescription = "object is in TRACKED_ZONE";
//                continue;
//            }
//        }
//        else {//object is in CAR_HOOD or GENERAL
//            trackedObject.zoneTypeIntersect = ZONE_TYPES::GENERAL;
//            trackedObject.warningPriority = WarningPriority::Safe;
//            trackedObject.warningDescription = "";
//        }
//    }
//}
//void NewPrediction::updateWarnings(std::unordered_map<int, TrackedObject>& predictedObjects)
//{
//    const std::pair<int, int>& imageBorders = IMAGE_BORDERS;
//    for (auto& [id, trackedObject] : predictedObjects) {
//        if (trackedObject.category == OBJECT_CATEGORY::TRAFFIC_LIGHT)
//            continue;
//        //CRITICAL_ZONE_1
//        else if (ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_1)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {
//            trackedObject.zoneTypeIntersect = ZONE_TYPES::CRITICAL_ZONE_1;
//            auto zoneTypeAndCatgoryPair = std::make_pair(ZONE_TYPES::CRITICAL_ZONE_1, trackedObject.category);
//            if (ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.find(zoneTypeAndCatgoryPair) != ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.end()) {
//                const auto& warningPriority_and_description = ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.at(zoneTypeAndCatgoryPair);
//                trackedObject.warningPriority = warningPriority_and_description.first;
//                trackedObject.warningDescription = warningPriority_and_description.second;
//            }
//            continue;
//        }
//        //CRITICAL_ZONE_2:
//        //if person...critical./if car : positive speed: safe/negative speed: high
//        else if (ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_2)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {
//            trackedObject.zoneTypeIntersect = ZONE_TYPES::CRITICAL_ZONE_2;
//            auto zoneTypeAndCatgoryPair = std::make_pair(ZONE_TYPES::CRITICAL_ZONE_2, trackedObject.category);
//            if (zoneTypeAndCatgoryPair.second == OBJECT_CATEGORY::PERSON && ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.find(zoneTypeAndCatgoryPair) != ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.end()) {
//                const auto& warningPriority_and_description = ZONE_OBJECT_TO_PRIORITY_DESCRIPTION_MAP.at(zoneTypeAndCatgoryPair);
//                trackedObject.warningPriority = warningPriority_and_description.first;
//                trackedObject.warningDescription = warningPriority_and_description.second;
//            }
//            else {
//                auto& distance = trackedObject.oldRelativSpeed;
//                if (distance >= 0) {
//                    trackedObject.warningPriority = WarningPriority::Safe;
//                    trackedObject.warningDescription = "Car in CRITICAL_ZONE_2";
//                }
//                else {
//                    trackedObject.warningPriority = WarningPriority::High;
//                    trackedObject.warningDescription = "Veichle is slowing down in CRITICAL_ZONE_2";
//                }
//            }
//            continue;
//        }
//        //TRACKED_ZONE
//        else if (ZONES_POLIGON[int(ZONE_TYPES::TRACKED_ZONE)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {
//            //color it in the 	TRACKED_ZONE color
//            trackedObject.zoneTypeIntersect = ZONE_TYPES::TRACKED_ZONE;
//            if (isApproachingDangerZone(ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_1)].first, trackedObject)) {
//                trackedObject.warningPriority = WarningPriority::High;
//                trackedObject.warningDescription = "object is move to CRITICAL_ZONE_1";
//                continue;
//            }
//            if (isApproachingDangerZone(ZONES_POLIGON[int(ZONE_TYPES::CRITICAL_ZONE_2)].first, trackedObject)) {
//                if (trackedObject.category == OBJECT_CATEGORY::PERSON) {
//                    trackedObject.warningPriority = WarningPriority::High;
//                    trackedObject.warningDescription = "person is move to CRITICAL_ZONE_2";
//                }
//                else {
//                    trackedObject.warningPriority = WarningPriority::Medium;
//                    trackedObject.warningDescription = "object is move to CRITICAL_ZONE_2";
//                }
//                continue;
//            }
//            else {//object is in TRACKED_ZONE but not in CRITICAL_ZONE_1 or CRITICAL_ZONE_2
//                trackedObject.warningPriority = WarningPriority::Low;
//                trackedObject.warningDescription = "object is in TRACKED_ZONE";
//                continue;
//            }
//        }
//        else {//object is in CAR_HOOD or GENERAL
//            trackedObject.zoneTypeIntersect = ZONE_TYPES::GENERAL;
//            trackedObject.warningPriority = WarningPriority::Safe;
//            trackedObject.warningDescription = "";
//        }
//    }
//}
/* if(trackedObject.category != OBJECT_CATEGORY::CAR && trackedObject.category != OBJECT_CATEGORY::PERSON && trackedObject.category != OBJECT_CATEGORY::BICYCLE
     && trackedObject.category != OBJECT_CATEGORY::MOTORBIKE && trackedObject.category != OBJECT_CATEGORY::TRUCK
     && trackedObject.category != OBJECT_CATEGORY::BUS && trackedObject.category != OBJECT_CATEGORY::TRAIN)
     continue;*/


//=================================================================================================
//std::pair<std::pair<cv::Point, cv::Point>, std::pair<cv::Point, cv::Point>> getMovementDirection(const cv::Rect& prevRect, const cv::Rect& currRect)
//{
//    const std::pair<int, int>& imageBorders = IMAGE_BORDERS;
//
//    std::pair<cv::Point, cv::Point> firstVector;
//    std::pair<cv::Point, cv::Point> secondVector;
//
//    cv::Point prevTL = prevRect.tl();
//    cv::Point currTL = currRect.tl();
//    cv::Point prevTR = cv::Point(prevRect.x + prevRect.width, prevRect.y);
//    cv::Point currTR = cv::Point(currRect.x + currRect.width, currRect.y);
//    cv::Point prevBL = cv::Point(prevRect.x, prevRect.y + prevRect.height);
//    cv::Point currBL = cv::Point(currRect.x, currRect.y + currRect.height);
//    cv::Point prevBR = prevRect.br();
//    cv::Point currBR = currRect.br();
//
//    int deltaX = currRect.x - prevRect.x;
//    int deltaY = currRect.y - prevRect.y;
//
//    if ((deltaX > 0 && deltaY > 0) || (deltaX < 0 && deltaY < 0)) {
//        firstVector = std::make_pair(prevTR, currTR);
//        secondVector = std::make_pair(prevBL, currBL);
//    }
//    else {
//        firstVector = std::make_pair(prevTL, currTL);
//        secondVector = std::make_pair(prevBR, currBR);
//    }
//    auto& cols = imageBorders.first;
//    auto& rows = imageBorders.second;
//    std::pair<cv::Point, cv::Point> imageUpperBorder = std::make_pair(cv::Point(0, 0), cv::Point(cols, 0));
//    std::pair<cv::Point, cv::Point> imageLowerBorder = std::make_pair(cv::Point(0, rows), cv::Point(cols, rows));
//    std::pair<cv::Point, cv::Point> imageLeftBorder = std::make_pair(cv::Point(0, 0), cv::Point(0, rows));
//    std::pair<cv::Point, cv::Point> imageRightBorder = std::make_pair(cv::Point(cols, 0), cv::Point(cols, rows));
//
//    if (deltaX > 0 && deltaY > 0) {
//        auto firstIntersection = findIntersection(firstVector.first, firstVector.second, imageLowerBorder.first, imageLowerBorder.second);
//        auto secondIntersection = findIntersection(secondVector.first, secondVector.second, imageLowerBorder.first, imageLowerBorder.second);
//
//        if (firstIntersection.has_value()) {
//            firstVector.second = firstIntersection.value();
//        }
//        else {
//            firstIntersection = findIntersection(firstVector.first, firstVector.second, imageRightBorder.first, imageRightBorder.second);
//            if (firstIntersection.has_value()) {
//                firstVector.second = firstIntersection.value();
//            }
//        }
//        if (secondIntersection.has_value()) {
//            secondVector.second = secondIntersection.value();
//        }
//        else {
//            secondIntersection = findIntersection(secondVector.first, secondVector.second, imageRightBorder.first, imageRightBorder.second);
//            if (secondIntersection.has_value()) {
//                secondVector.second = secondIntersection.value();
//            }
//        }
//
//    }
//    else if (deltaX < 0 && deltaY > 0) {
//        auto firstIntersection = findIntersection(firstVector.first, firstVector.second, imageLowerBorder.first, imageLowerBorder.second);
//        auto secondIntersection = findIntersection(secondVector.first, secondVector.second, imageLowerBorder.first, imageLowerBorder.second);
//
//        if (firstIntersection.has_value()) {
//            firstVector.second = firstIntersection.value();
//        }
//        else {
//            firstIntersection = findIntersection(firstVector.first, firstVector.second, imageLeftBorder.first, imageLeftBorder.second);
//            if (firstIntersection.has_value()) {
//                firstVector.second = firstIntersection.value();
//            }
//        }
//        if (secondIntersection.has_value()) {
//            secondVector.second = secondIntersection.value();
//        }
//        else {
//            secondIntersection = findIntersection(secondVector.first, secondVector.second, imageLeftBorder.first, imageLeftBorder.second);
//            if (secondIntersection.has_value()) {
//                secondVector.second = secondIntersection.value();
//            }
//        }
//    }
//    else if (deltaX > 0 && deltaY < 0) {
//        auto firstIntersection = findIntersection(firstVector.first, firstVector.second, imageUpperBorder.first, imageUpperBorder.second);
//        auto secondIntersection = findIntersection(secondVector.first, secondVector.second, imageUpperBorder.first, imageUpperBorder.second);
//
//        if (firstIntersection.has_value()) {
//            firstVector.second = firstIntersection.value();
//        }
//        else {
//            firstIntersection = findIntersection(firstVector.first, firstVector.second, imageRightBorder.first, imageRightBorder.second);
//            if (firstIntersection.has_value()) {
//                firstVector.second = firstIntersection.value();
//            }
//        }
//        if (secondIntersection.has_value()) {
//            secondVector.second = secondIntersection.value();
//        }
//        else {
//            secondIntersection = findIntersection(secondVector.first, secondVector.second, imageRightBorder.first, imageRightBorder.second);
//            if (secondIntersection.has_value()) {
//                secondVector.second = secondIntersection.value();
//            }
//        }
//    }
//    else if (deltaX < 0 && deltaY < 0) {
//        auto firstIntersection = findIntersection(firstVector.first, firstVector.second, imageUpperBorder.first, imageUpperBorder.second);
//        auto secondIntersection = findIntersection(secondVector.first, secondVector.second, imageUpperBorder.first, imageUpperBorder.second);
//
//        if (firstIntersection.has_value()) {
//            firstVector.second = firstIntersection.value();
//        }
//        else {
//            firstIntersection = findIntersection(firstVector.first, firstVector.second, imageLeftBorder.first, imageLeftBorder.second);
//            if (firstIntersection.has_value()) {
//                firstVector.second = firstIntersection.value();
//            }
//        }
//        if (secondIntersection.has_value()) {
//            secondVector.second = secondIntersection.value();
//        }
//        else {
//            secondIntersection = findIntersection(secondVector.first, secondVector.second, imageLeftBorder.first, imageLeftBorder.second);
//            if (secondIntersection.has_value()) {
//                secondVector.second = secondIntersection.value();
//            }
//        }
//    }
//    else if (deltaX == 0 && deltaY < 0) {
//        auto firstIntersection = findIntersection(firstVector.first, firstVector.second, imageUpperBorder.first, imageUpperBorder.second);
//        auto secondIntersection = findIntersection(secondVector.first, secondVector.second, imageUpperBorder.first, imageUpperBorder.second);
//
//        if (firstIntersection.has_value()) {
//            firstVector.second = firstIntersection.value();
//        }
//        if (secondIntersection.has_value()) {
//            secondVector.second = secondIntersection.value();
//        }
//    }
//    else if (deltaX == 0 && deltaY > 0) {
//        auto firstIntersection = findIntersection(firstVector.first, firstVector.second, imageLowerBorder.first, imageLowerBorder.second);
//        auto secondIntersection = findIntersection(secondVector.first, secondVector.second, imageLowerBorder.first, imageLowerBorder.second);
//
//        if (firstIntersection.has_value()) {
//            firstVector.second = firstIntersection.value();
//        }
//        if (secondIntersection.has_value()) {
//            secondVector.second = secondIntersection.value();
//        }
//    }
//    else if (deltaX > 0 && deltaY == 0) {
//        auto firstIntersection = findIntersection(firstVector.first, firstVector.second, imageLeftBorder.first, imageLeftBorder.second);
//        auto secondIntersection = findIntersection(secondVector.first, secondVector.second, imageLeftBorder.first, imageLeftBorder.second);
//
//        if (firstIntersection.has_value()) {
//            firstVector.second = firstIntersection.value();
//        }
//        if (secondIntersection.has_value()) {
//            secondVector.second = secondIntersection.value();
//        }
//    }
//    else if (deltaX < 0 && deltaY == 0) {
//        auto firstIntersection = findIntersection(firstVector.first, firstVector.second, imageRightBorder.first, imageRightBorder.second);
//        auto secondIntersection = findIntersection(secondVector.first, secondVector.second, imageRightBorder.first, imageRightBorder.second);
//
//        if (firstIntersection.has_value()) {
//            firstVector.second = firstIntersection.value();
//        }
//        if (secondIntersection.has_value()) {
//            secondVector.second = secondIntersection.value();
//        }
//    }
//
//    return std::make_pair(firstVector, secondVector);
//}
//=================================================================================================
///**
//* @brief Adjust Both Vectors Based on Movement:
//* Handles the adjustment of both vectors depending on the movement direction.
//*/
//void NewPrediction::adjustVectors(
//    std::pair<cv::Point, cv::Point>& firstVector,
//    std::pair<cv::Point, cv::Point>& secondVector,
//    int deltaX, int deltaY) {
//
//    std::pair<cv::Point, cv::Point> imageUpperBorder = std::make_pair(cv::Point(0, 0), cv::Point(IMAGE_BORDERS.first, 0));
//    std::pair<cv::Point, cv::Point> imageLowerBorder = std::make_pair(cv::Point(0, IMAGE_BORDERS.second), cv::Point(IMAGE_BORDERS.first, IMAGE_BORDERS.second));
//    std::pair<cv::Point, cv::Point> imageLeftBorder = std::make_pair(cv::Point(0, 0), cv::Point(0, IMAGE_BORDERS.second));
//    std::pair<cv::Point, cv::Point> imageRightBorder = std::make_pair(cv::Point(IMAGE_BORDERS.first, 0), cv::Point(IMAGE_BORDERS.first, IMAGE_BORDERS.second));
//
//    if (deltaX > 0 && deltaY > 0) {
//        bool isFirstIntersect = adjustVectorForIntersection(firstVector, imageLowerBorder);
//        bool isSecondIntersect = adjustVectorForIntersection(secondVector, imageLowerBorder);
//
//        if (!isFirstIntersect)
//            adjustVectorForIntersection(firstVector, imageRightBorder);
//        if (!isSecondIntersect)
//            adjustVectorForIntersection(secondVector, imageRightBorder);
//    }
//    else if (deltaX < 0 && deltaY > 0) {
//        bool isFirstIntersect = adjustVectorForIntersection(firstVector, imageLowerBorder);
//        bool isSecondIntersect = adjustVectorForIntersection(secondVector, imageLowerBorder);
//
//        if (!isFirstIntersect)
//            adjustVectorForIntersection(firstVector, imageLeftBorder);
//        if (!isSecondIntersect)
//            adjustVectorForIntersection(secondVector, imageLeftBorder);
//    }
//    else if (deltaX > 0 && deltaY < 0) {
//        bool isFirstIntersect = adjustVectorForIntersection(firstVector, imageUpperBorder);
//        bool isSecondIntersect = adjustVectorForIntersection(secondVector, imageUpperBorder);
//
//        if (!isFirstIntersect)
//            adjustVectorForIntersection(firstVector, imageRightBorder);
//        if (!isSecondIntersect)
//            adjustVectorForIntersection(secondVector, imageRightBorder);
//    }
//    else if (deltaX < 0 && deltaY < 0) {
//        bool isFirstIntersect = adjustVectorForIntersection(firstVector, imageUpperBorder);
//        bool isSecondIntersect = adjustVectorForIntersection(secondVector, imageUpperBorder);
//
//        if (!isFirstIntersect)
//            adjustVectorForIntersection(firstVector, imageLeftBorder);
//        if (!isSecondIntersect)
//            adjustVectorForIntersection(secondVector, imageLeftBorder);
//    }
//    else if (deltaX == 0 && deltaY < 0) {
//        adjustVectorForIntersection(firstVector, imageUpperBorder);
//        adjustVectorForIntersection(secondVector, imageUpperBorder);
//    }
//    else if (deltaX == 0 && deltaY > 0) {
//        adjustVectorForIntersection(firstVector, imageLowerBorder);
//        adjustVectorForIntersection(secondVector, imageLowerBorder);
//    }
//    else if (deltaX > 0 && deltaY == 0) {
//        adjustVectorForIntersection(firstVector, imageLeftBorder);
//        adjustVectorForIntersection(secondVector, imageLeftBorder);
//    }
//    else if (deltaX < 0 && deltaY == 0) {
//        adjustVectorForIntersection(firstVector, imageRightBorder);
//        adjustVectorForIntersection(secondVector, imageRightBorder);
//    }
//}
////=================================================================================================
///**
//* @brief Adjust Both Vectors Based on Movement:
//* Handles the adjustment of both vectors depending on the movement direction.
//*/
//void NewPrediction::adjustVectors(
//    std::pair<cv::Point, cv::Point>& firstVector,
//    std::pair<cv::Point, cv::Point>& secondVector,
//    int deltaX, int deltaY) {
//
//    std::pair<cv::Point, cv::Point> imageUpperBorder = std::make_pair(cv::Point(0, 0), cv::Point(IMAGE_BORDERS.first, 0));
//    std::pair<cv::Point, cv::Point> imageLowerBorder = std::make_pair(cv::Point(0, IMAGE_BORDERS.second), cv::Point(IMAGE_BORDERS.first, IMAGE_BORDERS.second));
//    std::pair<cv::Point, cv::Point> imageLeftBorder = std::make_pair(cv::Point(0, 0), cv::Point(0, IMAGE_BORDERS.second));
//    std::pair<cv::Point, cv::Point> imageRightBorder = std::make_pair(cv::Point(IMAGE_BORDERS.first, 0), cv::Point(IMAGE_BORDERS.first, IMAGE_BORDERS.second));
//
//    if (deltaX > 0)
//    {
//        if (deltaY > 0)
//        {
//            bool isFirstIntersect = adjustVectorForIntersection(firstVector, imageLowerBorder);
//            bool isSecondIntersect = adjustVectorForIntersection(secondVector, imageLowerBorder);
//
//            if (!isFirstIntersect)
//                adjustVectorForIntersection(firstVector, imageRightBorder);
//            if (!isSecondIntersect)
//                adjustVectorForIntersection(secondVector, imageRightBorder);
//        }
//        else if (deltaY < 0)
//        {
//            bool isFirstIntersect = adjustVectorForIntersection(firstVector, imageUpperBorder);
//            bool isSecondIntersect = adjustVectorForIntersection(secondVector, imageUpperBorder);
//
//            if (!isFirstIntersect)
//                adjustVectorForIntersection(firstVector, imageRightBorder);
//            if (!isSecondIntersect)
//                adjustVectorForIntersection(secondVector, imageRightBorder);
//        }
//        else//deltaY == 0
//        {
//            adjustVectorForIntersection(firstVector, imageLeftBorder);
//            adjustVectorForIntersection(secondVector, imageLeftBorder);
//        }
//    }
//    else if (deltaX < 0)
//    {
//        if (deltaY > 0)
//        {
//            bool isFirstIntersect = adjustVectorForIntersection(firstVector, imageLowerBorder);
//            bool isSecondIntersect = adjustVectorForIntersection(secondVector, imageLowerBorder);
//
//            if (!isFirstIntersect)
//                adjustVectorForIntersection(firstVector, imageLeftBorder);
//            if (!isSecondIntersect)
//                adjustVectorForIntersection(secondVector, imageLeftBorder);
//        }
//        else if (deltaY < 0)
//        {
//            bool isFirstIntersect = adjustVectorForIntersection(firstVector, imageUpperBorder);
//            bool isSecondIntersect = adjustVectorForIntersection(secondVector, imageUpperBorder);
//
//            if (!isFirstIntersect)
//                adjustVectorForIntersection(firstVector, imageLeftBorder);
//            if (!isSecondIntersect)
//                adjustVectorForIntersection(secondVector, imageLeftBorder);
//        }
//        else//deltaY == 0
//        {
//            adjustVectorForIntersection(firstVector, imageRightBorder);
//            adjustVectorForIntersection(secondVector, imageRightBorder);
//        }
//    }
//    else//deltaX == 0
//    {
//        if (deltaY < 0)
//        {
//            adjustVectorForIntersection(firstVector, imageUpperBorder);
//            adjustVectorForIntersection(secondVector, imageUpperBorder);
//        }
//        else if (deltaY > 0)
//        {
//            adjustVectorForIntersection(firstVector, imageLowerBorder);
//            adjustVectorForIntersection(secondVector, imageLowerBorder);
//        }
//    }
//}
////=================================================================================================
///**
//* @brief Adjust Both Vectors Based on Movement:
//* Handles the adjustment of both vectors depending on the movement direction.
//*/
//void NewPrediction::adjustVectors(
//    std::pair<cv::Point, cv::Point>& firstVector,
//    std::pair<cv::Point, cv::Point>& secondVector,
//    int deltaX, int deltaY) {
//
//    std::pair<cv::Point, cv::Point> imageUpperBorder = std::make_pair(cv::Point(0, 0), cv::Point(IMAGE_BORDERS.first, 0));
//    std::pair<cv::Point, cv::Point> imageLowerBorder = std::make_pair(cv::Point(0, IMAGE_BORDERS.second), cv::Point(IMAGE_BORDERS.first, IMAGE_BORDERS.second));
//    std::pair<cv::Point, cv::Point> imageLeftBorder = std::make_pair(cv::Point(0, 0), cv::Point(0, IMAGE_BORDERS.second));
//    std::pair<cv::Point, cv::Point> imageRightBorder = std::make_pair(cv::Point(IMAGE_BORDERS.first, 0), cv::Point(IMAGE_BORDERS.first, IMAGE_BORDERS.second));
//
//    if (deltaX > 0)
//    {
//        if (deltaY == 0)
//        {
//            adjustVectorForIntersection(firstVector, imageLeftBorder);
//            adjustVectorForIntersection(secondVector, imageLeftBorder);
//            return;
//        }
//
//        bool isFirstIntersect, isSecondIntersect;
//        if (deltaY > 0)
//        {
//            isFirstIntersect = adjustVectorForIntersection(firstVector, imageLowerBorder);
//            isSecondIntersect = adjustVectorForIntersection(secondVector, imageLowerBorder);
//        }
//        else//deltaY < 0
//        {
//            isFirstIntersect = adjustVectorForIntersection(firstVector, imageUpperBorder);
//            isSecondIntersect = adjustVectorForIntersection(secondVector, imageUpperBorder);
//        }
//        if (!isFirstIntersect)
//            adjustVectorForIntersection(firstVector, imageRightBorder);
//        if (!isSecondIntersect)
//            adjustVectorForIntersection(secondVector, imageRightBorder);
//    }
//    else if (deltaX < 0)
//    {
//        if (deltaY == 0)
//        {
//            adjustVectorForIntersection(firstVector, imageRightBorder);
//            adjustVectorForIntersection(secondVector, imageRightBorder);
//            return;
//        }
//
//        bool isFirstIntersect, isSecondIntersect;
//        if (deltaY > 0)
//        {
//            isFirstIntersect = adjustVectorForIntersection(firstVector, imageLowerBorder);
//            isSecondIntersect = adjustVectorForIntersection(secondVector, imageLowerBorder);
//        }
//        else//deltaY < 0
//        {
//            isFirstIntersect = adjustVectorForIntersection(firstVector, imageUpperBorder);
//            isSecondIntersect = adjustVectorForIntersection(secondVector, imageUpperBorder);
//        }
//        if (!isFirstIntersect)
//            adjustVectorForIntersection(firstVector, imageLeftBorder);
//        if (!isSecondIntersect)
//            adjustVectorForIntersection(secondVector, imageLeftBorder);
//    }
//    else//deltaX == 0
//    {
//        if (deltaY > 0)
//        {
//            adjustVectorForIntersection(firstVector, imageLowerBorder);
//            adjustVectorForIntersection(secondVector, imageLowerBorder);
//        }
//        else if (deltaY < 0)
//        {
//            adjustVectorForIntersection(firstVector, imageUpperBorder);
//            adjustVectorForIntersection(secondVector, imageUpperBorder);
//        }
//    }
//}