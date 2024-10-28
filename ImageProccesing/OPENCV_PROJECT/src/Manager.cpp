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
    Visualization::drawPredictedObjects(frame, m_predictedObjects);

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
            if(predictedObject.warningPriority != WarningPriority::Safe)
                Visualization::drawWarningDescription(frame, predictedObject);
        }
    }


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
void Manager::runTracking() {
    const auto& videoZonesPoligons = InputData::getInstance().getAllVideosPoligons();

    for (const auto& [videoName, _] : videoZonesPoligons)
    {
        NewPrediction::setCurrentVideoPoligons(videoName);
		Visualization::setCurrentVideoPoligons(videoName);
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

