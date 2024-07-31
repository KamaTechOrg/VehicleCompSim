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

void detectVehiclesV3(cv::Mat& frame, cv::dnn::Net& net, const std::vector<std::string>& classList) {
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
				if (maxClassScore >= 0.5 && classList[classIdPoint.x] == "car") {
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
	for (int idx : indices) {
		cv::Rect box = boxes[idx];
		cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
		std::string label = classList[classIds[idx]];
		int baseLine;
		cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
		int top = cv::max(box.y, labelSize.height);
		cv::putText(frame, label, cv::Point(box.x, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
	}
}


int run_yolov3(cv::Mat& original_img)
{
	std::string cfgFile = "yolov3.cfg";
	std::string weightsFile = "yolov3.weights";
	std::string classFile = "coco.names";

	std::vector<std::string> classList = v3loadClassList(classFile);
	cv::dnn::Net net = loadNetV3(cfgFile, weightsFile);

	cv::Mat& image = original_img;
	detectVehiclesV3(image, net, classList);

	//cv::imshow("Vehicles found", image);
	//cv::waitKey(0);


	return 0;
}


//
//
//#include <iostream>
//#include <fstream>
//
//#include "open_cv.h"
//using namespace cv;
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
//cv::dnn::Net loadNet(const std::string& cfgFile, const std::string& weightsFile) {
//    return cv::dnn::readNetFromDarknet(cfgFile, weightsFile);
//}
//
//void detectVehicles(cv::Mat& frame, cv::dnn::Net& net, const std::vector<std::string>& classList) {
//    cv::Mat blob;
//    cv::dnn::blobFromImage(frame, blob, 1.0 / 255.0, cv::Size(416, 416), cv::Scalar(), true, false);
//    net.setInput(blob);
//    std::vector<cv::Mat> outs;
//    net.forward(outs, net.getUnconnectedOutLayersNames());
//
//    std::vector<int> classIds;
//    std::vector<float> confidences;
//    std::vector<cv::Rect> boxes;
//
//    for (const auto& out : outs) {
//        auto* data = (float*)out.data;
//        for (int i = 0; i < out.rows; ++i, data += out.cols) {
//            float confidence = data[4];
//            if (confidence >= 0.5) {
//                cv::Mat scores = out.row(i).colRange(5, out.cols);
//                cv::Point classIdPoint;
//                double maxClassScore;
//                cv::minMaxLoc(scores, 0, &maxClassScore, 0, &classIdPoint);
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
//                    boxes.push_back(cv::Rect(left, top, width, height));
//                }
//            }
//        }
//    }
//
//    std::vector<int> indices;
//    cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);
//    for (int idx : indices) {
//        cv::Rect box = boxes[idx];
//        cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
//        std::string label = classList[classIds[idx]];
//        int baseLine;
//        cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
//        int top = cv::max(box.y, labelSize.height);
//        cv::putText(frame, label, cv::Point(box.x, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
//    }
//}
//
//int main() {
//    std::string imagePath = R"(C:\Users\dmaro\Downloads\cars.jpg)";
//    std::string cfgFile = "yolov3.cfg";
//    std::string weightsFile = "yolov3.weights";
//    std::string classFile = "coco.names";
//
//    std::vector<std::string> classList = loadClassList(classFile);
//    cv::dnn::Net net = loadNet(cfgFile, weightsFile);
//
//    cv::Mat image = cv::imread(imagePath);
//    if (image.empty()) {
//        std::cerr << "Error loading image: " << imagePath << std::endl;
//        return -1;
//    }
//
//    detectVehicles(image, net, classList);
//
//    cv::imshow("Vehicles found", image);
//    cv::waitKey(0);
//
//    return 0;
//}


