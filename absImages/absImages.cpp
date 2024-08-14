#ifdef _DEBUG
// the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world454d.lib")
#else
// the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world454.lib")
#endif

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <filesystem>
#include <chrono>

using namespace std;

bool doRectanglesOverlap(const cv::Rect& rect1, const cv::Rect& rect2, int threshold) {
	int x1 = std::max(rect1.x, rect2.x);
	int y1 = std::max(rect1.y, rect2.y);
	int x2 = std::min(rect1.x + rect1.width, rect2.x + rect2.width);
	int y2 = std::min(rect1.y + rect1.height, rect2.y + rect2.height);

	if (x1 < x2 && y1 < y2) {
		return true;  // מלבנים חופפים
	}

	// בדיקה אם המלבנים קרובים מספיק
	return (std::abs(rect1.x - rect2.x) < threshold &&
		std::abs(rect1.y - rect2.y) < threshold);
}

cv::Rect mergeRectangles(const cv::Rect& rect1, const cv::Rect& rect2) {
	int x = std::min(rect1.x, rect2.x);
	int y = std::min(rect1.y, rect2.y);
	int width = std::max(rect1.x + rect1.width, rect2.x + rect2.width) - x;
	int height = std::max(rect1.y + rect1.height, rect2.y + rect2.height) - y;
	return cv::Rect(x, y, width, height);
}

std::vector<cv::Rect> mergeOverlappingRectangles(const std::vector<cv::Rect>& rects, int threshold) {
	std::vector<cv::Rect> mergedRects = rects;
	bool merged;
	do {
		merged = false;
		for (auto it1 = mergedRects.begin(); it1 != mergedRects.end(); ++it1) {
			for (auto it2 = std::next(it1); it2 != mergedRects.end();) {
				if (doRectanglesOverlap(*it1, *it2, threshold)) {
					*it1 = mergeRectangles(*it1, *it2);
					it2 = mergedRects.erase(it2);
					merged = true;
				}
				else {
					++it2;
				}
			}
		}
	} while (merged);
	return mergedRects;
}

std::vector<cv::Rect> detectChanges(const cv::Mat& img1, const cv::Mat& img2, int threshold = 30, int mergeThreshold = 60, int minDimension = 5) {
	// וידוא שהתמונות באותו גודל
	if (img1.size() != img2.size()) {
		throw std::runtime_error("התמונות חייבות להיות באותו גודל");
	}

	// המרה לגווני אפור
	cv::Mat gray1, gray2;
	cv::cvtColor(img1, gray1, cv::COLOR_BGR2GRAY);
	cv::cvtColor(img2, gray2, cv::COLOR_BGR2GRAY);

	// חישוב ההפרש המוחלט
	cv::Mat diff;
	cv::absdiff(gray1, gray2, diff);

	//cv::imshow("Detected Changes", diff);
	//cv::waitKey();

	// סינון רעשים באמצעות סף
	cv::Mat thresh;
	cv::threshold(diff, thresh, threshold, 255, cv::THRESH_BINARY);

	//cv::imshow("Detected Changes", thresh);
	//cv::waitKey();

	// מציאת קונטורים
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// חישוב מלבנים מוכלים וסינון לפי מידה מינימלית
	std::vector<cv::Rect> rois;
	for (const auto& contour : contours) {
		cv::Rect rect = cv::boundingRect(contour);
		if (rect.width >= minDimension && rect.height >= minDimension) {
			rois.push_back(rect);
		}
	}

	rois = mergeOverlappingRectangles(rois, mergeThreshold);

	//// המרת תמונת הסף לתמונה צבעונית כדי שנוכל לצייר עליה מלבנים צבעוניים
	//cv::Mat threshColor;
	//cv::cvtColor(thresh, threshColor, cv::COLOR_GRAY2BGR);

	//// ציור המלבנים על התמונה
	//for (const auto& roi : rois) {
	//	cv::rectangle(threshColor, roi, cv::Scalar(0, 255, 0), 2); // ירוק, עובי 2
	//}

	//// הצגת התמונה כולל המלבנים
	//cv::imshow("Detected Changes", threshColor);
	//cv::waitKey();

	return rois;
}


void run_a_folder(string path_to_folder) {
	try {
		std::vector<std::filesystem::path> imagePaths;

		// איסוף כל קבצי התמונה מהתיקייה
		for (const auto& entry : std::filesystem::directory_iterator(path_to_folder)) {
			imagePaths.push_back(entry.path());
		}

		// מיון הנתיבים לפי שם הקובץ
		std::sort(imagePaths.begin(), imagePaths.end());

		// קריאה של התמונה הראשונה
		cv::Mat prevImage = cv::imread(imagePaths[0].string());
		if (prevImage.empty()) {
			std::cerr << "ERROR with reading the image " << imagePaths[0] << std::endl;
			return;
		}

		// עיבוד זוגות תמונות עוקבות
		for (size_t i = 1; i < imagePaths.size(); ++i) {
			cv::Mat currentImage = cv::imread(imagePaths[i].string());

			if (currentImage.empty()) {
				std::cerr << "ERROR with reading the image : " << imagePaths[i] << std::endl;
				continue;
			}

			std::vector<cv::Rect> changes = detectChanges(prevImage, currentImage);

			std::cout << "changes betweeen pict " << imagePaths[i - 1].filename() << " to " << imagePaths[i].filename() << ": " << changes.size() << std::endl;

			// אם תרצה להדפיס את המלבנים, הסר את ההערה מהשורות הבאות
			/*
			for (const auto& roi : changes) {
				std::cout << "  " << roi.x << ", " << roi.y << ", " << roi.width << ", " << roi.height << std::endl;
			}
			*/

			// העברת התמונה הנוכחית להיות התמונה הקודמת בסיבוב הבא
			prevImage = currentImage;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "שגיאה: " << e.what() << std::endl;
		return;
	}
}


void run_2_pict(string path_img1, string path_img2) {
	try {
		// טעינת התמונות
		cv::Mat img1 = cv::imread(path_img1);
		cv::Mat img2 = cv::imread(path_img2);
		if (img1.empty() || img2.empty()) {
			throw std::runtime_error("שגיאה בטעינת התמונות");
		}

		// מדידת זמן עבור הפונקציה detectChanges
		auto start = std::chrono::high_resolution_clock::now();
		std::vector<cv::Rect> changes = detectChanges(img1, img2);
		auto end = std::chrono::high_resolution_clock::now();

		// חישוב הזמן שעבר במילישניות
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		std::cout << "changes betweeen pict " << path_img1 << " to " << path_img2 << ": " << changes.size() << std::endl;
		std::cout << "Time taken by detectChanges: " << duration.count() << " milliseconds" << std::endl;

		//std::cout << "prints the rect in format (x, y, width, height):" << std::endl;
		//for (const auto& roi : changes) {
		//    std::cout << roi.x << ", " << roi.y << ", " << roi.width << ", " << roi.height << std::endl;
		//}
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return;
	}
}

// דוגמת הרצה, נדרש להחליף את הנתיבים לתמונות
int main() {
	std::string imagesFolderPath = R"(C:\Users\Dell\Downloads\extracted_frames_20240814_144845)";
	std::string img1Path = R"(C:\Users\Dell\Downloads\datasets\vehicles.v2-release.tensorflow\test\aditganteng_mp4-325_jpg.rf.b36c2a3f5ffee114908b7c09aa90adbc.jpg)";
	std::string img2Path = R"(C:\Users\Dell\Downloads\datasets\vehicles.v2-release.tensorflow\test\aditganteng_mp4-329_jpg.rf.49efa46ce5033447fe3695e020f59df0.jpg)";

	//run_a_folder(imagesFolderPath);
	run_2_pict(img1Path, img2Path);

	return 0;
}