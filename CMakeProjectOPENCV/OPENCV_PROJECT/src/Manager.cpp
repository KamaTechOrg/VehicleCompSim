#include "Manager.h"
#include "Visualization.h"
#include "ApproachDangerZone.h"
Manager::Manager(const std::string& videoPath, const std::string& videoDir)
    : m_videoPath(videoPath), m_videoDir(videoDir) {}


//=============================================================================
float calculateCentroidDistance(const cv::Rect& r1, const cv::Rect& r2) 
{
    cv::Point2f c1(r1.x + r1.width / 2.0f, r1.y + r1.height / 2.0f);
    cv::Point2f c2(r2.x + r2.width / 2.0f, r2.y + r2.height / 2.0f);
    return cv::norm(c1 - c2); // Euclidean distance between centroids
}
//=============================================================================
double calculateIoU(const cv::Rect& rect1, const cv::Rect& rect2) {
    cv::Rect intersection = rect1 & rect2;
    double intersectionArea = static_cast<double>(intersection.area());
    double unionArea = static_cast<double>(rect1.area() + rect2.area() - intersectionArea);
    return (unionArea > 0) ? intersectionArea / unionArea : 0.0;
}

//=============================================================================
void matchOverlapBoxes(std::unordered_map<int, TrackedObject>& OldPredictedObjects, std::unordered_map<int, TrackedObject>& NewPredictedObjects) {
    for (auto& [id, newPredictedObject] : NewPredictedObjects) {
        int maxAreaId = -1;
        int maxArea = 0;
        for (auto& [id, oldPredictedObject] : OldPredictedObjects) {
            if(newPredictedObject.category != oldPredictedObject.category)continue;
            double areaIntersection = calculateIoU(newPredictedObject.bboxCurr, oldPredictedObject.bboxCurr);
            if(areaIntersection > MIN_INTERSECTION_AREA && areaIntersection > maxArea)
			{
				maxArea = areaIntersection;
				maxAreaId = id;
			}			
        }
        if (maxAreaId != -1) {
            newPredictedObject.distancePrev = OldPredictedObjects[maxAreaId].distanceCurr;
            OldPredictedObjects.erase(maxAreaId);
        }
        else {
            int minDistance = 1000000;
            int minDistanceId = -1;
            for (auto& [id, oldPredictedObject] : OldPredictedObjects) {
                if (newPredictedObject.category != oldPredictedObject.category)continue;
                double distance = calculateCentroidDistance(newPredictedObject.bboxCurr, oldPredictedObject.bboxCurr);
                if(distance < CENTER_DISTANCE_THRESHOLD && distance < minDistance){
                    minDistance = distance;
					minDistanceId = id;
				}
            }
            if (minDistanceId != -1) {
                newPredictedObject.distancePrev = OldPredictedObjects[minDistanceId].distanceCurr;
                OldPredictedObjects.erase(minDistanceId);
            }
        }
        
    }
}
//=============================================================================
void Manager::runTracking()
{
    cv::VideoCapture cap(m_videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video." << std::endl;
        return;
    }

    std::string videoName = std::filesystem::path(m_videoPath).stem().string();// Extract video name from path

    cv::Mat frame;
    int frameCount = 0;
    int frameInterval = 6;

    while (cap.read(frame)) {
        if(frameCount < FRAME_BEGIN){
			++frameCount;
			continue;
		}

        std::string frameName = "Frame " + std::to_string(frameCount);
        int currentFrameIndex = frameCount / frameInterval;

        // שמירת הפריים הראשון כתמונה במחשב
        if (frameCount == FRAME_BEGIN) {
            std::string outputImagePath = "first_frame.png"; // ניתן לשנות את שם הקובץ והנתיב
            cv::imwrite(outputImagePath, frame);
            std::cout << "First frame saved to: " << outputImagePath << std::endl;
        }


        std::unordered_map<int, TrackedObject>& predictedObjects = m_prediction.getPredictedObjects();
        if (frameCount % FRAMES_NUM_UNTIL_INIT_TRACKERS == 0)
        {
            auto detectedObjects = run_yolov3(frame, frameName);
            std::unordered_map<int, TrackedObject> predictedObjectsCopy = predictedObjects;
            m_trackerManager.initializeTrackers(frame, detectedObjects, predictedObjects);
            m_prediction.setCanPredict(false);
            matchOverlapBoxes(predictedObjectsCopy, predictedObjects);
        }
        else
        {
            m_trackerManager.updateTrackers(frame, predictedObjects);
            m_prediction.setCanPredict(true);
        }
        std::unordered_set<int> toColor;
        if (m_prediction.getCanPredict()) {
            //toColor = m_prediction.analyzeSituation(frame.rows * frame.cols);
            /*std::unordered_set<int> approachingObjectsID = ApproachDangerZone::getApproachingDangerZoneIDs(predictedObjects);
            toColor.insert(approachingObjectsID.begin(), approachingObjectsID.end());*/
            NewPrediction::updateWarnings(predictedObjects);
        }
        Distance::calcDist(predictedObjects, frame);


        if (frameCount % FRAMES_NUM_UNTIL_INIT_TRACKERS == 0) {
            Distance::calcRelativSpeed(predictedObjects, frame, true);
            for(auto& [id, predictedObject] : predictedObjects) {
                //Visualization::drawRelativeSpeedWithDistances(frame, predictedObject);
            }

        }
        else {
            Distance::calcRelativSpeed(predictedObjects, frame, false);
            for (auto& [id, predictedObject] : predictedObjects) {
                //Visualization::drawRelativeSpeed(frame, predictedObject, predictedObject.oldRelativSpeed);
            }
        }

        //TEMPORARY: ONLY FOR VISUALIZATION!!!!!!!!!!!!!!
        Visualization::drawPredictedObjects(frame, predictedObjects);
        //Visualization::drawCarsHoodTarpezoid(frame, predictedObjects);
        for (auto& [id, predictedObject] : predictedObjects) {
            Visualization::drawWarningDescription(frame, predictedObject);
        }
        Visualization::drawZones(frame);

        cv::imshow("Tracking", frame);

        if (frameCount % FRAMES_NUM_UNTIL_INIT_TRACKERS == 0) {
            cv::waitKey(1);
        }
        else {
            cv::waitKey(1);
        }

        for (auto& [id, predictedObject] : predictedObjects)
        {
            predictedObject.bboxPrev = predictedObject.bboxCurr;
        }
        ++frameCount;
    }

    cap.release();
    cv::destroyAllWindows();
}