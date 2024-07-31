#include "open_cv.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "yoloV4-tiny.h"


using namespace cv;
using namespace cv::dnn;


std::vector<std::string> loadClassList(const std::string& filePath) {
    std::vector<std::string> classList;
    std::ifstream ifs(filePath);
    std::string line;
    while (std::getline(ifs, line)) {
        classList.push_back(line);
    }
    return classList;
}

Net loadNet(const std::string& cfgFile, const std::string& weightsFile) {
    return dnn::readNetFromDarknet(cfgFile, weightsFile);
}

int detectVehicles(Mat& frame, Net& net, const std::vector<std::string>& classList) {
    Mat blob;
    dnn::blobFromImage(frame, blob, 1.0 / 255.0, Size(416, 416), Scalar(), true, false);
    net.setInput(blob);
    std::vector<Mat> outs;
    net.forward(outs, net.getUnconnectedOutLayersNames());

    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<Rect> boxes;

    for (const auto& out : outs) {
        auto* data = (float*)out.data;
        for (int i = 0; i < out.rows; ++i, data += out.cols) {
            float confidence = data[4];
            if (confidence >= 0.5) {
                Mat scores = out.row(i).colRange(5, out.cols);
                Point classIdPoint;
                double maxClassScore;
                minMaxLoc(scores, 0, &maxClassScore, 0, &classIdPoint);
                if (maxClassScore >= 0.5 && classList[classIdPoint.x] == "car") {
                    int centerX = static_cast<int>(data[0] * frame.cols);
                    int centerY = static_cast<int>(data[1] * frame.rows);
                    int width = static_cast<int>(data[2] * frame.cols);
                    int height = static_cast<int>(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back(static_cast<float>(maxClassScore));
                    boxes.push_back(Rect(left, top, width, height));
                }
            }
        }
    }

    std::vector<int> indices;
    dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);

    int carCount = 0;
    for (int idx : indices) {
        Rect box = boxes[idx];
        rectangle(frame, box, Scalar(0, 255, 0), 2);
        std::string label = classList[classIds[idx]];
        int baseLine;
        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        int top = std::max(box.y, labelSize.height);
        putText(frame, label, Point(box.x, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);

        if (label == "car") {
            carCount++;
        }
    }

    return carCount;
}

int run_yoloV4Tiny(cv::Mat& original_img) {
    std::string cfgFile = "yolov4-tiny.cfg";
    std::string weightsFile = "yolov4-tiny.weights";
    std::string classFile = "coco.names";

    static std::vector<std::string> classList = loadClassList(classFile);
    static Net net = loadNet(cfgFile, weightsFile);

    Mat image = original_img;

    int carCount = detectVehicles(image, net, classList);
    std::cout << "Number of cars detected: " << carCount << std::endl;

    //imshow("Detected Vehicles", image);
    //waitKey(0);

    return carCount;
}







//#pragma once
//#include "open_cv.h"
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <chrono>
//
//#include <opencv2/opencv.hpp>
//
//
//using namespace cv;
//using namespace cv::dnn;
//
//std::vector<std::string> loadClassList(const std::string& filePath) {
//    std::vector<std::string> classList;
//    std::ifstream ifs(filePath);
//    std::string line;
//    while (std::getline(ifs, line)) {
//        classList.push_back(line);
//    }
//    return classList;
//}
//
//Net loadNet(const std::string& cfgFile, const std::string& weightsFile) {
//    return dnn::readNetFromDarknet(cfgFile, weightsFile);
//}
//
//void detectVehicles(Mat& frame, Net& net, const std::vector<std::string>& classList) {
//    Mat blob;
//    dnn::blobFromImage(frame, blob, 1.0 / 255.0, Size(416, 416), Scalar(), true, false);
//    net.setInput(blob);
//    std::vector<Mat> outs;
//    net.forward(outs, net.getUnconnectedOutLayersNames());
//
//    std::vector<int> classIds;
//    std::vector<float> confidences;
//    std::vector<Rect> boxes;
//
//    for (const auto& out : outs) {
//        auto* data = (float*)out.data;
//        for (int i = 0; i < out.rows; ++i, data += out.cols) {
//            float confidence = data[4];
//            if (confidence >= 0.5) {
//                Mat scores = out.row(i).colRange(5, out.cols);
//                Point classIdPoint;
//                double maxClassScore;
//                minMaxLoc(scores, 0, &maxClassScore, 0, &classIdPoint);
//                if (maxClassScore >= 0.5 && classList[classIdPoint.x] == "car") {
//                    int centerX = static_cast<int>(data[0] * frame.cols);
//                    int centerY = static_cast<int>(data[1] * frame.rows);
//                    int width = static_cast<int>(data[2] * frame.cols);
//                    int height = static_cast<int>(data[3] * frame.rows);
//                    int left = centerX - width / 2;
//                    int top = centerY - height / 2;
//
//                    classIds.push_back(classIdPoint.x);
//                    confidences.push_back(static_cast<float>(maxClassScore));
//                    boxes.push_back(Rect(left, top, width, height));
//                }
//            }
//        }
//    }
//
//    std::vector<int> indices;
//    dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);
//    for (int idx : indices) {
//        Rect box = boxes[idx];
//        rectangle(frame, box, Scalar(0, 255, 0), 2);
//        std::string label = classList[classIds[idx]];
//        int baseLine;
//        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
//        int top = std::max(box.y, labelSize.height);
//        putText(frame, label, Point(box.x, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
//    }
//}
//
//int main() {
//    std::string imagePath = R"(C:\Users\dmaro\Downloads\cars.jpg)";
//    std::string cfgFile = "yolov4-tiny.cfg";
//    std::string weightsFile = "yolov4-tiny.weights";
//    std::string classFile = "coco.names";
//
//    std::vector<std::string> classList = loadClassList(classFile);
//    Net net = loadNet(cfgFile, weightsFile);
//
//    Mat image = imread(imagePath);
//    if (image.empty()) {
//        std::cerr << "Error loading image: " << imagePath << std::endl;
//        return -1;
//    }
//    const auto start = std::chrono::steady_clock::now();
//    detectVehicles(image, net, classList);
//    const auto end = std::chrono::steady_clock::now();
//    const std::chrono::duration<double> elapsed_seconds = end - start;
//    std::cout << std::endl << "____TIME___:  " << elapsed_seconds.count() << "s\n"; // Before C++20
//    //std::cout << elapsed_seconds << '\n'; // C++20's chrono::duration operator<<
//
//    imshow("Vehicles found", image);
//    waitKey(0);
//
//    return 0;
//}


