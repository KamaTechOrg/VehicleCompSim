#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <opencv2/core/types.hpp>
#include <filesystem>
#include <opencv2/imgcodecs.hpp>
#include "absImage.h"
#include <iostream>

struct Annotation {
	int width, height;
	std::string className;
	cv::Rect bbox;
};

std::unordered_map<std::string, std::vector<Annotation>> readAnnotations(const std::string& filename) {
	std::unordered_map<std::string, std::vector<Annotation>> annotations;
	std::ifstream file(filename);
	std::string line;

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string imageName;
		Annotation ann;

		if (std::getline(iss, imageName, ',') &&
			iss >> ann.width &&
			iss.ignore() >> ann.height &&
			std::getline(iss.ignore(), ann.className, ',') &&
			iss >> ann.bbox.x &&
			iss.ignore() >> ann.bbox.y &&
			iss.ignore() >> ann.bbox.width &&
			iss.ignore() >> ann.bbox.height) {

			annotations[imageName].push_back(ann);
		}
	}

	return annotations;
}

bool isRectContained(const cv::Rect& outer, const cv::Rect& inner) {
	return (inner.x >= outer.x && inner.y >= outer.y &&
		inner.x + inner.width <= outer.x + outer.width &&
		inner.y + inner.height <= outer.y + outer.height);
}

void testDetectChanges(const std::string& annotationFile, const std::string& imageFolder) {
	auto annotations = readAnnotations(annotationFile);
	int totalTestRects = 0, totalDetectedRects = 0, containedRects = 0;

	for (const auto& entry : std::filesystem::directory_iterator(imageFolder)) {
		if (entry.path().extension() != ".jpg" && entry.path().extension() != ".png") continue;

		std::string baseName = entry.path().stem().string();
		std::string nextImageName = baseName.substr(0, baseName.length() - 1) +
			std::to_string(std::stoi(baseName.substr(baseName.length() - 1)) + 1) +
			entry.path().extension().string();

		if (!std::filesystem::exists(entry.path().parent_path() / nextImageName)) continue;

		cv::Mat img1 = cv::imread(entry.path().string());
		cv::Mat img2 = cv::imread((entry.path().parent_path() / nextImageName).string());

		// Filter out stationary objects
		std::vector<cv::Rect> movingObjects;
		for (const auto& ann1 : annotations[baseName]) {
			bool found = false;
			for (const auto& ann2 : annotations[nextImageName]) {
				if (ann1.bbox == ann2.bbox && ann1.className == ann2.className) {
					found = true;
					break;
				}
			}
			if (!found) movingObjects.push_back(ann1.bbox);
		}

		// Merge overlapping rectangles from annotations
		std::vector<cv::Rect> mergedTestRects = mergeOverlappingRectangles(movingObjects, 30);

		// Detect changes
		std::vector<cv::Rect> detectedRects = detectChanges(img1, img2);

		totalTestRects += mergedTestRects.size();
		totalDetectedRects += detectedRects.size();

		// Check if detected rectangles contain the test rectangles
		for (const auto& testRect : mergedTestRects) {
			for (const auto& detectedRect : detectedRects) {
				if (isRectContained(detectedRect, testRect)) {
					containedRects++;
					break;
				}
			}
		}
	}

	double rectRatio = static_cast<double>(totalDetectedRects) / totalTestRects;
	double containmentRate = static_cast<double>(containedRects) / totalTestRects;

	std::cout << "Rectangle ratio (detected/test): " << rectRatio << std::endl;
	std::cout << "Containment rate: " << containmentRate << std::endl;
}
//
//int main() {
//
//	std::string imagesFolderPath = R"(C:\Users\Dell\Downloads\datasets\vehicles.v2-release.tensorflow\test)";
//	std::string anotationPath = R"(C:\Users\Dell\Downloads\datasets\vehicles.v2-release.tensorflow\_annotations.csv)";
//
//	testDetectChanges(anotationPath, imagesFolderPath);
//	return 0;
//}