#pragma once
#include "constants.h"
#include "tracking.h"
#include "yolov3.h"
#include <filesystem>
#include <unordered_map>
#include "Prediction.h"
#include "Distance.h"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include "NewPrediction.h"
#include "SafeQueue.h"

class Manager
{
public:
	Manager(SafeQueue& queue, const std::string& videoPath = VIDEO_PATH, const std::string& videoDir = VIDEO_DIR);
	void runTracking();
private:
	std::string m_videoPath;
	std::string m_videoDir;
	TrackerManager m_trackerManager;
	Prediction m_prediction;
	SafeQueue& m_queue;
	//Distance m_distance;
};

