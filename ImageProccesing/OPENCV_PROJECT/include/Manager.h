#pragma once
#include "constants.h"
#include "tracking.h"
#include "yolov3.h"
#include <filesystem>
#include <unordered_map>
#include "Distance.h"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include "NewPrediction.h"
//class SafeQueue;

class Manager
{
public:
	Manager(SafeQueue& queue, const std::string& videoPath = VIDEO_PATH, const std::string& videoDir = VIDEO_DIR);
	void runTracking();
private:
	std::string m_videoPath;
	std::string m_videoDir;
	TrackerManager m_trackerManager;
	std::unordered_map<int, TrackedObject> m_predictedObjects;
	SafeQueue& m_queue;
	//Distance m_distance;


	cv::VideoCapture openVideo();
	void processFrame(cv::Mat& frame, int frameCount);
	void saveFirstFrame(const cv::Mat& frame);
	void detectAndInitializeTrackers(cv::Mat& frame);
	void updateTrackersAndPredict(cv::Mat& frame);
	void drawPredictedObjectsAndWarnings(cv::Mat& frame, bool toRunDetection);
};

