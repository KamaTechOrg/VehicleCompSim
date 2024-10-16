#pragma once

#include <string>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/dnn.hpp"
#include <opencv2/objdetect.hpp>
#include "constants.h"


std::vector<std::pair<cv::Rect, std::string>> run_yolov3(cv::Mat& original_img);
