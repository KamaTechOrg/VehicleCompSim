#include "Manager.h"
#include "Visualization.h"
#include "sendWarning.h"
#include "InputData.h"


//=============================================================================
Manager::Manager(SafeQueue& queue)
    : m_queue(queue), m_videoDir(InputData::getInstance().getVideosDirPath()) {}
//=============================================================================
/**
* @brief Open Video:
* Handles opening the video and returning a cv::VideoCapture object.
*/
cv::VideoCapture Manager::openVideo() {
    cv::VideoCapture cap(m_videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video." << std::endl;
    }
    return cap;
}
//=============================================================================
/**
* @brief Process Frame:
* Processes each frame, applying object detection and tracking.
*/
void Manager::processFrame(cv::Mat& frame, int frameCount) {
    if (SAVE_FIRST_IMG && frameCount == FRAME_BEGIN) {
        saveFirstFrame(frame);
    }
    bool toRunDetection = frameCount % FRAMES_NUM_UNTIL_INIT_TRACKERS == 0;
    if (toRunDetection) {
        detectAndInitializeTrackers(frame);
    }
    else {
        updateTrackersAndPredict(frame);
    }

    //update distance and relative speed
    Distance::calcDist(m_predictedObjects, frame);
    Distance::calcRelativSpeed(m_predictedObjects, frame, toRunDetection);

    if constexpr(ENABLE_DRAWING)
        drawPredictedObjectsAndWarnings(frame, toRunDetection);

    for (auto& [id, predictedObject] : m_predictedObjects) {
        if (int(predictedObject.warningPriority) <= 2) {
            sendWarningToConsole(predictedObject.warningDescription);
            m_queue.enqueue(predictedObject.warningDescription);
        }
    }
    for (auto& [id, predictedObject] : m_predictedObjects)
    {
        predictedObject.bboxPrev = predictedObject.bboxCurr;
    }
}

//=============================================================================
/**
* @brief Save First Frame:
* Saves the first frame.
*/
void Manager::saveFirstFrame(const cv::Mat& frame) {
    std::string outputImagePath = "first_frame.png";
    cv::imwrite(outputImagePath, frame);
    std::cout << "First frame saved to: " << outputImagePath << std::endl;
}
//=============================================================================
/**
* @brief Detect and Initialize Trackers:
* Runs YOLO detection and initializes trackers.
*/
void Manager::detectAndInitializeTrackers(cv::Mat& frame) {
    auto detectedObjects = run_yolov3(frame);
    std::unordered_map<int, TrackedObject> predictedObjectsCopy = m_predictedObjects;
    m_trackerManager.initializeTrackers(frame, detectedObjects, m_predictedObjects);
    NewPrediction::matchOverlapBoxes(predictedObjectsCopy, m_predictedObjects);
}
//=============================================================================
/**
* @brief Update Trackers and Predict:
* Updates trackers and makes predictions.
*/
void Manager::updateTrackersAndPredict(cv::Mat& frame) {
    m_trackerManager.updateTrackers(frame, m_predictedObjects);

    //std::unordered_set<int> toColor;
    //toColor = m_prediction.analyzeSituation(frame.rows * frame.cols);
    //std::unordered_set<int> approachingObjectsID = ApproachDangerZone::getApproachingDangerZoneIDs(predictedObjects);
    //toColor.insert(approachingObjectsID.begin(), approachingObjectsID.end());
    NewPrediction::updateWarnings(m_predictedObjects);
}
//=============================================================================
/**
* @brief Draw Predicted Objects and Warnings:
* Draws predicted objects and their associated warnings.
*/
void Manager::drawPredictedObjectsAndWarnings(cv::Mat& frame, bool toRunDetection) {
    if constexpr(DRAW_RELATIVE_SPEED)
    {
        for (auto& [id, predictedObject] : m_predictedObjects) {
            Visualization::drawRelativeSpeed(frame, predictedObject, predictedObject.oldRelativSpeed);
        }
    }
    else if constexpr (DRAW_RELATIVE_SPEED_WITH_DISTANCE)
    {
        for (auto& [id, predictedObject] : m_predictedObjects) {
            Visualization::drawRelativeSpeedWithDistances(frame, predictedObject);
        }
    }
    else if constexpr (DRAW_WARNINGS_MSG)
    {
        for (auto& [id, predictedObject] : m_predictedObjects) {
            Visualization::drawWarningDescription(frame, predictedObject);
        }
    }

    Visualization::drawPredictedObjects(frame, m_predictedObjects);
    Visualization::drawZones(frame);

    cv::imshow("Tracking", frame);
    if (toRunDetection) {
        cv::waitKey(WAIT_KEY_SEC_DETECTION);
    }
    else {
        cv::waitKey(WAIT_KEY_SEC_TRACKERS);
    }
}
//=============================================================================
void Manager::resetMembers(const std::string& videoName)
{
    m_videoPath = m_videoDir + "\\" + videoName;
    m_trackerManager = TrackerManager();
    m_predictedObjects.clear();
    m_queue.clearQueue();
}
//=============================================================================
//void Manager::runTracking() {
//    cv::VideoCapture cap = openVideo();
//    if (!cap.isOpened()) return;
//    cap.set(cv::CAP_PROP_POS_FRAMES, FRAME_BEGIN - 1);
//
//    int frameCount = FRAME_BEGIN;
//    cv::Mat frame;
//
//    while (cap.read(frame)) {
//        processFrame(frame, frameCount);
//        ++frameCount;
//    }
//
//    cap.release();
//    cv::destroyAllWindows();
//}
//=============================================================================
void Manager::runTracking() {
    const auto& videoZonesPoligons = InputData::getInstance().getAllVideosPoligons();

    for (const auto& [videoName, _] : videoZonesPoligons)
    {
        NewPrediction::setCurrentVideoPoligons(videoName);
        resetMembers(videoName);

        cv::VideoCapture cap = openVideo();
        if (!cap.isOpened()) return;
        cap.set(cv::CAP_PROP_POS_FRAMES, FRAME_BEGIN - 1);

        int frameCount = FRAME_BEGIN;
        cv::Mat frame;

        while (cap.read(frame)) {
            processFrame(frame, frameCount);
            ++frameCount;
        }

        cap.release();
        cv::destroyAllWindows();
    }
}
//=============================================================================
//void Manager::matchOverlapBoxes(std::unordered_map<int, TrackedObject>& OldPredictedObjects, std::unordered_map<int, TrackedObject>& NewPredictedObjects) {
//    for (auto& [id, newPredictedObject] : NewPredictedObjects) {
//        int maxAreaId = -1;
//        int maxArea = 0;
//        for (auto& [id, oldPredictedObject] : OldPredictedObjects) {
//            if(newPredictedObject.category != oldPredictedObject.category)continue;
//            double areaIntersection = calculateIoU(newPredictedObject.bboxCurr, oldPredictedObject.bboxCurr);
//            if(areaIntersection > MIN_INTERSECTION_AREA && areaIntersection > maxArea)
//			{
//				maxArea = int(areaIntersection);
//				maxAreaId = id;
//			}			
//        }
//        if (maxAreaId != -1) {
//            newPredictedObject.distancePrev = OldPredictedObjects[maxAreaId].distanceCurr;
//            OldPredictedObjects.erase(maxAreaId);
//        }
//        else {
//            int minDistance = 1000000;
//            int minDistanceId = -1;
//            for (auto& [id, oldPredictedObject] : OldPredictedObjects) {
//                if (newPredictedObject.category != oldPredictedObject.category)continue;
//                double distance = calculateCentroidDistance(newPredictedObject.bboxCurr, oldPredictedObject.bboxCurr);
//                if(distance < CENTER_DISTANCE_THRESHOLD && distance < minDistance){
//                    minDistance = int(distance);
//					minDistanceId = id;
//				}
//            }
//            if (minDistanceId != -1) {
//                newPredictedObject.distancePrev = OldPredictedObjects[minDistanceId].distanceCurr;
//                OldPredictedObjects.erase(minDistanceId);
//            }
//        }
//        
//    }
//}
//=============================================================================
//void Manager::runTracking()
//{
//    cv::VideoCapture cap(m_videoPath);
//    if (!cap.isOpened()) {
//        std::cerr << "Error: Could not open video." << std::endl;
//        return;
//    }
//
//    cv::Mat frame;
//    int frameCount = 0;
//
//    while (cap.read(frame)) {
//        if (frameCount < FRAME_BEGIN) {
//            ++frameCount;
//            continue;
//        }
//
//        if (frameCount == FRAME_BEGIN) {
//            std::string outputImagePath = "first_frame.png";
//            cv::imwrite(outputImagePath, frame);
//            std::cout << "First frame saved to: " << outputImagePath << std::endl;
//        }
//
//
//        std::unordered_map<int, TrackedObject>& predictedObjects = m_prediction.getPredictedObjects();
//        if (frameCount % FRAMES_NUM_UNTIL_INIT_TRACKERS == 0)
//        {
//            auto detectedObjects = run_yolov3(frame);
//            std::unordered_map<int, TrackedObject> predictedObjectsCopy = predictedObjects;
//            m_trackerManager.initializeTrackers(frame, detectedObjects, predictedObjects);
//            m_prediction.setCanPredict(false);
//            matchOverlapBoxes(predictedObjectsCopy, predictedObjects);
//        }
//        else
//        {
//            m_trackerManager.updateTrackers(frame, predictedObjects);
//            m_prediction.setCanPredict(true);
//        }
//        std::unordered_set<int> toColor;
//        if (m_prediction.getCanPredict()) {
//            //toColor = m_prediction.analyzeSituation(frame.rows * frame.cols);
//            /*std::unordered_set<int> approachingObjectsID = ApproachDangerZone::getApproachingDangerZoneIDs(predictedObjects);
//            toColor.insert(approachingObjectsID.begin(), approachingObjectsID.end());*/
//            NewPrediction::updateWarnings(predictedObjects);
//        }
//        Distance::calcDist(predictedObjects, frame);
//
//
//        if (frameCount % FRAMES_NUM_UNTIL_INIT_TRACKERS == 0) {
//            Distance::calcRelativSpeed(predictedObjects, frame, true);
//            for (auto& [id, predictedObject] : predictedObjects) {
//                if (ENABLE_DRAWING)
//                {
//                    //Visualization::drawRelativeSpeedWithDistances(frame, predictedObject);
//                }
//            }
//
//        }
//        else {
//            Distance::calcRelativSpeed(predictedObjects, frame, false);
//            for (auto& [id, predictedObject] : predictedObjects) {
//                if (ENABLE_DRAWING)
//                {
//                    //Visualization::drawRelativeSpeed(frame, predictedObject, predictedObject.oldRelativSpeed);
//                }
//            }
//        }
//
//        if (ENABLE_DRAWING)
//        {
//            Visualization::drawPredictedObjects(frame, predictedObjects);
//            //Visualization::drawCarsHoodTarpezoid(frame, predictedObjects);        
//        }
//
//        for (auto& [id, predictedObject] : predictedObjects) {
//            if (ENABLE_DRAWING)
//            {
//                Visualization::drawWarningDescription(frame, predictedObject);
//            }
//
//            if (int(predictedObject.warningPriority) <= 2)
//            {
//                sendWarningToConsole(predictedObject.warningDescription);
//                //m_queue.enqueue(predictedObject.warningDescription);
//            }
//        }
//
//        if (ENABLE_DRAWING)
//        {
//            Visualization::drawZones(frame);
//            cv::imshow("Tracking", frame);
//
//            if (frameCount % FRAMES_NUM_UNTIL_INIT_TRACKERS == 0) {
//                cv::waitKey(1);
//            }
//            else {
//                cv::waitKey(1);
//            }
//        }
//
//        for (auto& [id, predictedObject] : predictedObjects)
//        {
//            predictedObject.bboxPrev = predictedObject.bboxCurr;
//        }
//        ++frameCount;
//    }
//
//    cap.release();
//    cv::destroyAllWindows();
//}
//=============================================================================


