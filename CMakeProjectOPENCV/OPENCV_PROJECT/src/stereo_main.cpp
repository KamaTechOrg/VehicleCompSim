//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <vector>
//#include <utility> // for std::pair
//#include <string>
//#include <filesystem>
//#include "yolov3.h"
//
//// פונקציה לטעינת כל הקבצים מתיקייה
//std::vector<std::string> load_image_filenames(const std::string& folder) {
//    std::vector<std::string> filenames;
//
//    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
//        if (entry.is_regular_file()) {
//            filenames.push_back(entry.path().filename().string());
//        }
//    }
//
//    std::sort(filenames.begin(), filenames.end());
//    return filenames;
//}
//
//// פונקציה לחישוב disparity בשיטה אפיפולרית
//double calculate_distance_epipolar(const cv::Mat& left_image, const cv::Mat& right_image, const cv::Rect& bbox_left, double focal_length_px, double baseline_m) {
//    // יצירת אובייקט StereoBM לצורך חישוב disparity
//    cv::Ptr<cv::StereoBM> stereo = cv::StereoBM::create(16, 9); // 16 = מספר רמות disparity, 9 = גודל חלון בלוק
//
//    // המרה לגווני אפור
//    cv::Mat left_gray, right_gray;
//    cv::cvtColor(left_image, left_gray, cv::COLOR_BGR2GRAY);
//    cv::cvtColor(right_image, right_gray, cv::COLOR_BGR2GRAY);
//
//    // חישוב disparity בין התמונות השמאלית והימנית
//    cv::Mat disparity;
//    stereo->compute(left_gray, right_gray, disparity);
//
//    // ניקוי רעשים מה-disparity
//    disparity.convertTo(disparity, CV_32F, 1.0 / 1.0); // המרה לפורמט float32 כדי להקטין את ערכי ה-disparity
//
//    // חילוץ מרכז תיבת הזיהוי (bounding box) של האובייקט
//    cv::Point bbox_center(bbox_left.x + bbox_left.width / 2, bbox_left.y + bbox_left.height / 2);
//
//    // חישוב ה-disparity בנקודה המרכזית של האובייקט
//    float disparity_value = disparity.at<float>(bbox_center.y, bbox_center.x);
//
//    // בדיקה אם ה-disparity תקף
//    if (disparity_value <= 0.0) {
//        return -1.0; // Invalid disparity, return negative distance
//    }
//
//    // חישוב המרחק לפי disparity
//    return (focal_length_px * baseline_m) / disparity_value;
//}
//
//int main() {
//    std::string left_folder = R"(C:\Users\dmaro\Downloads\tracking_train4_v1.1\argoverse-tracking\train4\91326240-9132-9132-9132-591327440896\stereo_front_left)";
//    std::string right_folder = R"(C:\Users\dmaro\Downloads\tracking_train4_v1.1\argoverse-tracking\train4\91326240-9132-9132-9132-591327440896\stereo_front_right)";
//    double focal_length_px = 3757.0; // pixel
//    double baseline_m = 0.2986; // meter
//
//    std::vector<std::string> left_images = load_image_filenames(left_folder);
//    std::vector<std::string> right_images = load_image_filenames(right_folder);
//
//    if (left_images.size() != right_images.size()) {
//        std::cerr << "Error: Number of left and right images must be the same!" << std::endl;
//        return -1;
//    }
//
//    size_t start_index = 0;
//
//    for (size_t i = start_index; i < left_images.size(); ++i) {
//        std::string left_image_path = left_folder + "/" + left_images[i];
//        std::string right_image_path = right_folder + "/" + right_images[i];
//
//        cv::Mat left_image = cv::imread(left_image_path);
//        cv::Mat right_image = cv::imread(right_image_path);
//
//        if (left_image.empty() || right_image.empty()) {
//            std::cerr << "Error loading images: " << left_image_path << " or " << right_image_path << std::endl;
//            continue;
//        }
//
//        // הפעלת YOLOv3 על התמונה השמאלית
//        std::vector<std::pair<cv::Rect, std::string>> objects_left = run_yolov3(left_image, left_images[i]);
//
//        // חישוב המרחק לכל אובייקט בתמונה השמאלית
//        for (const auto& obj : objects_left) {
//            const cv::Rect& bbox_left = obj.first;
//
//            // חישוב מרחק באמצעות חישוב אפיפולרי
//            double distance_m = calculate_distance_epipolar(left_image, right_image, bbox_left, focal_length_px, baseline_m);
//
//            // הדפסת נתוני האובייקט והמרחק
//            std::string distanceLabel = cv::format("%.2f m", distance_m);
//            int fontFace = cv::FONT_HERSHEY_SIMPLEX;
//            double fontScale = 0.7;
//            int thickness = 2;
//            int baseline = 0;
//            cv::Size textSize = cv::getTextSize(distanceLabel, fontFace, fontScale, thickness, &baseline);
//            cv::Point textOrg(bbox_left.x, bbox_left.y - textSize.height - 10); // Adjusted position
//
//            // ציור רקע לבן מאחורי הכיתוב
//            cv::rectangle(left_image, textOrg + cv::Point(0, baseline), textOrg + cv::Point(textSize.width, -textSize.height), cv::Scalar(255, 255, 255), cv::FILLED);
//            // ציור הכיתוב
//            cv::putText(left_image, distanceLabel, textOrg, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
//
//            if (distance_m > 0) {
//                std::cout << "Object: " << obj.second << ", Distance: " << distance_m << " meters." << std::endl;
//            }
//            else {
//                std::cout << "Object: " << obj.second << ", could not calculate distance (disparity issue)." << std::endl;
//            }
//        }
//
//        cv::namedWindow("Left Image with Detected Objects", cv::WINDOW_NORMAL);
//        //cv::namedWindow("Right Image with Detected Objects", cv::WINDOW_NORMAL);
//
//        // שינוי גודל חלונות בהתאם לגודל התמונות
//        cv::resizeWindow("Left Image with Detected Objects", (left_image.cols),( left_image.rows));
//        //cv::resizeWindow("Right Image with Detected Objects", right_image.cols / 4, (right_image.rows) / 4);
//
//        cv::imshow("Left Image with Detected Objects", left_image);
//        //cv::imshow("Right Image with Detected Objects", right_image);
//
//        cv::waitKey(1);
//    }
//
//    return 0;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
