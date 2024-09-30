#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "yolov3.h"
#include "util.h"

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

std::vector<std::pair<cv::Rect, std::string>> detectVehiclesV3(cv::Mat& frame, cv::dnn::Net& net, const std::vector<std::string>& classList) {
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
                if(maxClassScore >= 0.5 && (
                    classList[classIdPoint.x] == OBJECT_CATEGORY::CAR ||
                    classList[classIdPoint.x] == OBJECT_CATEGORY::PERSON ||
                    classList[classIdPoint.x] == OBJECT_CATEGORY::TRAFFIC_LIGHT ||
                    classList[classIdPoint.x] == OBJECT_CATEGORY::BICYCLE ||
                    classList[classIdPoint.x] == OBJECT_CATEGORY::TRUCK ||
                    classList[classIdPoint.x] == OBJECT_CATEGORY::BUS ||
                    classList[classIdPoint.x] == OBJECT_CATEGORY::MOTORBIKE ||
                    classList[classIdPoint.x] == OBJECT_CATEGORY::TRAIN
                    )) {
                    int centerX = static_cast<int>(data[0] * frame.cols);
                    int centerY = static_cast<int>(data[1] * frame.rows);
                    int width = static_cast<int>(data[2] * frame.cols);
                    int height = static_cast<int>(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back(static_cast<float>(maxClassScore));
                    boxes.push_back(cv::Rect(left, top, width, height));
                }
            }
        }
    }

    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);
    std::vector<std::pair<cv::Rect, std::string>> resultBoxes;
    for (int idx : indices) {
        cv::Rect box = boxes[idx];
        if ((box.y + box.height + 30) > frame.rows && box.width >= (frame.cols - 30))
            continue;
        resultBoxes.push_back(std::make_pair(box, classList[classIds[idx]]));
    }
    return resultBoxes;
}

std::vector<std::pair<cv::Rect, std::string>> run_yolov3(cv::Mat& original_img, std::string& image_name)
{
    static std::string cfgFile = DATA_DIR "\\weights\\yolov3.cfg";
    static std::string weightsFile = DATA_DIR "\\weights\\yolov3.weights";
    static std::string classFile = DATA_DIR "\\weights\\coco.names";

    static std::vector<std::string> classList = v3loadClassList(classFile);
    static cv::dnn::Net net = loadNetV3(cfgFile, weightsFile);

    cv::Mat& image = original_img;
    std::vector<std::pair<cv::Rect, std::string>> detectedBoxes = detectVehiclesV3(image, net, classList);

    return detectedBoxes;
}