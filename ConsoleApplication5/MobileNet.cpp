#include "MobileNet.h"

#include <iostream>
#include <vector>
#include "open_cv.h"
using namespace cv;
using namespace cv::dnn;


int run_MobileNet(cv::Mat& original_img)
{
    std::string prototxt = "MobileNetSSD_deploy.prototxt";
    std::string model = "MobileNetSSD_deploy.caffemodel";

    Net net = dnn::readNetFromCaffe(prototxt, model);

    Mat& frame = original_img;
    Mat blob = dnn::blobFromImage(frame, 0.007843, Size(300, 300), 127.5, false, false);
    net.setInput(blob);
    Mat detection = net.forward();

    Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
    int counter = 0;
    for (int i = 0; i < detectionMat.rows; i++) {
        float confidence = detectionMat.at<float>(i, 2);
        if (confidence > 0.5) {
            counter++;
            int idx = static_cast<int>(detectionMat.at<float>(i, 1));
            int left = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
            int top = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
            int right = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
            int bottom = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
            rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 255, 0), 2);
        }
    }
    
    //imshow("Detection", frame);
    //waitKey(0);
    return counter;
}





