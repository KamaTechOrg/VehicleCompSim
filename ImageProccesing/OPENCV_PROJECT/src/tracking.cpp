#include "tracking.h"
//=============================================================================
void TrackerManager::initializeTrackers(const cv::Mat& frame, std::vector<std::pair<cv::Rect, std::string>>& detectedResult, std::unordered_map<int, TrackedObject>& predictedObjects)
{
    trackers.clear();
    predictedObjects.clear();
    
    for(int id = 0; id < detectedResult.size(); id++)
	{
        cv::Rect2d bbox(detectedResult[id].first.x, detectedResult[id].first.y, detectedResult[id].first.width, detectedResult[id].first.height);
        cv::Ptr<cv::Tracker> tracker = cv::TrackerKCF::create();
        tracker->init(frame, bbox);
        trackers[id] = tracker;
		predictedObjects[id] = TrackedObject(detectedResult[id].second, detectedResult[id].first);
	}
}

//=============================================================================
/**
* @brief   updates each tracker with the new frame, providing an updated bounding box.
           If a tracker fails to update (e.g., if the object is lost), you can
           handle that situation, such as by removing the tracker.
* @param   trackers
* @param   frame
* @param   updatedBoxes
*/
void TrackerManager::updateTrackers(const cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects)
{
    std::vector<int> toRemove;
    
    for (auto& [id, tracker] : trackers) {
        cv::Rect bbox;
        bool ok = tracker->update(frame, bbox);

        if (ok)
        {
            predictedObjects[id].bboxCurr = bbox;
            //predictedObjects[trackerPair.first].distancePrev = predictedObjects[trackerPair.first].distanceCurr;
        }
        else
        {
            toRemove.push_back(id);
        }
    }

    for (int id : toRemove) {
        trackers.erase(id);
        predictedObjects.erase(id);
    }
}

//=============================================================================
