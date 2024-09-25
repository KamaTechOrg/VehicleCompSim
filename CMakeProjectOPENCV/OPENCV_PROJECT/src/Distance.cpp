#include "Distance.h"
#include "constants.h"
#include "Visualization.h"

float Distance::estimateDistanceWithWidth(float objectWidthInPixels, std::string& category)
{
    float objectWidth = OBJECTS_HEIGHT_WIDTH.at(category).second;
    return (objectWidth * FOCAL_LENGTH) / objectWidthInPixels;
}

float Distance::estimateDistanceWithHeight(float objectHeightInPixels, std::string& category)
{
    float objectHeight = OBJECTS_HEIGHT_WIDTH.at(category).first;
    return (objectHeight * FOCAL_LENGTH) / objectHeightInPixels;
}

void Distance::calcDist(std::unordered_map<int, TrackedObject>& predictedObjects, cv::Mat& frame)
{
    for (auto& [id, predictedObject] : predictedObjects) {
        if (predictedObject.category != "car" && predictedObject.category != "person"
            && predictedObject.category != "truck" && predictedObject.category  != "bus")
            continue;
        cv::Rect rect = predictedObject.bboxCurr;
        std::string category = predictedObject.category;
        float distance;
        bool isSquare = (float(rect.width) / float(rect.height)) <= SQUER_RATIO_LIMIT &&
            (float(rect.height) / float(rect.width)) <= SQUER_RATIO_LIMIT;
        if (isSquare)
            distance = estimateDistanceWithWidth(static_cast<float>(rect.width), category) - CARMERA_TO_CAR_BOUNDARIS;
        else
            distance = estimateDistanceWithHeight(static_cast<float>(rect.height), category) - CARMERA_TO_CAR_BOUNDARIS;

        predictedObjects[id].distanceCurr = distance;

        // הכנת המידע להצגה
        //Visualization::drawDistance(frame, rect, distance);
    }
}

void Distance::calcRelativSpeed(std::unordered_map<int, TrackedObject>& predictedObjects, cv::Mat& frame, bool isDetection)
{
    for (const auto& [id, predictedObject] : predictedObjects) {
        if (!isDetection && predictedObject.oldRelativSpeed != -1 && predictedObject.distancePrev != -1 && predictedObject.distanceCurr != -1) {
            auto& relativSpeed = predictedObject.oldRelativSpeed;
            
            //float deltaDistance = predictedObject.distancePrev - predictedObject.distanceCurr;
            //float deltaTime = 18.0f / 30.18f;
            //float relativSpeed = deltaDistance / deltaTime;

            // הכנת המידע להצגה
            //Visualization::drawRelativeSpeed(frame, predictedObject, relativSpeed);
        }
        else if (isDetection && predictedObject.distancePrev != -1) {
            float deltaDistance = predictedObject.distanceCurr - predictedObject.distancePrev;
            float deltaTime = float(FRAMES_NUM_UNTIL_INIT_TRACKERS) / VIDEO_FPS;
            float relativSpeed = deltaDistance / deltaTime;
            predictedObjects[id].oldRelativSpeed = relativSpeed;

            // הכנת המידע להצגה
            //Visualization::drawRelativeSpeedWithDistances(frame, predictedObject, relativSpeed);
        }
    }
}




