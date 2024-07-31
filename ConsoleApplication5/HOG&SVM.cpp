#include "open_cv.h"
#include <iostream>
#include "HOG&SVM.h"

using namespace cv;

int run_HOG(cv::Mat& original_img) {
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector()); // השתמש ב-HOG SVM Detector שמגיע עם OpenCV

    Mat& image = original_img;


    std::vector<Rect> detections;
    std::vector<double> weights;

    hog.detectMultiScale(image, detections, weights);

    for (const auto& rect : detections) {
        std::cout<< "Vehicle detected at: " << rect << std::endl;
        rectangle(image, rect, Scalar(0, 255, 0), 2);
    }

    //imshow("Detected Vehicles", image);
    //waitKey(0);

    return detections.size();
}
