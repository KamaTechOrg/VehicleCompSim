#include <iostream>
#include <fstream>
#include <vector>
#include "open_cv.h"
#include "yolov3.h"
using namespace cv;


std::vector<std::string> v3loadClassList(const std::string& filePath) {
    std::vector<std::string> classList;
    std::ifstream ifs(filePath);
    std::string line;
    while (std::getline(ifs, line)) {
        classList.push_back(line);
    }
    return classList;
}

cv::dnn::Net loadNetV3(const std::string& cfgFile, const std::string& weightsFile) {
    return cv::dnn::readNetFromDarknet(cfgFile, weightsFile);
}

std::vector<cv::Rect> detectObjectsV3(cv::Mat& frame, cv::dnn::Net& net, const std::vector<std::string>& classList) {
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1.0 / 255.0, cv::Size(416, 416), cv::Scalar(), true, false);

    net.setInput(blob);
    std::vector<cv::Mat> outs;
    net.forward(outs, net.getUnconnectedOutLayersNames());

    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;



    for (const auto& out : outs) {
        auto* data = (float*)out.data;
        for (int i = 0; i < out.rows; ++i, data += out.cols) {
            float confidence = data[4];
            if (confidence >= 0.5) {
                cv::Mat scores = out.row(i).colRange(5, out.cols);
                cv::Point classIdPoint;
                double maxClassScore;
                cv::minMaxLoc(scores, 0, &maxClassScore, 0, &classIdPoint);
                if (maxClassScore >= 0.5 && (classList[classIdPoint.x] == "car" ||
                    classList[classIdPoint.x] == "person" ||
                    classList[classIdPoint.x] == "traffic light" ||
                    classList[classIdPoint.x] == "bicycle" ||
                    classList[classIdPoint.x] == "truck" ||
                    classList[classIdPoint.x] == "bus" ||
                    classList[classIdPoint.x] == "motorbike" ||
                    classList[classIdPoint.x] == "train"
                    )) {
                    int centerX = static_cast<int>(data[0] * frame.cols);
                    int centerY = static_cast<int>(data[1] * frame.rows);
                    int width = static_cast<int>(data[2] * frame.cols);
                    int height = static_cast<int>(data[3] * frame.rows);
                    int left = std::max(0, centerX - width / 2);
                    int top = std::max(0, centerY - height / 2);
                    int right = std::min(frame.cols - 1, left + width);
                    int bottom = std::min(frame.rows - 1, top + height);

                    width = right - left;
                    height = bottom - top;

                    // Add the box only if it's valid
                    if (width > 0 && height > 0) {
                        classIds.push_back(classIdPoint.x);
                        confidences.push_back(static_cast<float>(maxClassScore));
                        boxes.push_back(cv::Rect(left, top, width, height));
                    }
                }
            }
        }
    }

    std::vector<cv::Rect> result_boxes;
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);
    for (int idx : indices) {
        cv::Rect box = boxes[idx];
        result_boxes.push_back(box);
        /*cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
        std::string label = classList[classIds[idx]];
        int baseLine;
        cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        int top = cv::max(box.y, labelSize.height);
        cv::putText(frame, label, cv::Point(box.x, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);*/
    }
    return result_boxes;
}

std::vector<cv::Rect> run_yolov3(cv::Mat& original_img) {
    static std::string cfgFile = "yolov3.cfg";
    static std::string weightsFile = "yolov3.weights";
    static std::string classFile = "coco.names";

    static std::vector<std::string> classList = v3loadClassList(classFile);
    static cv::dnn::Net net = loadNetV3(cfgFile, weightsFile);

    cv::Mat& image = original_img;
    std::vector<cv::Rect> result_bexes = detectObjectsV3(image, net, classList);

    //cv::imshow("Objects found", image);
    //cv::waitKey(0);

    return result_bexes;
}
