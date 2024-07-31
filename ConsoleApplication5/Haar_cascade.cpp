#include "Haar_cascade.h"
using namespace std;

int run_haar_cascade(cv::Mat& original_img)
{
    // Load the trained cascade model for car identifyng
    cv::CascadeClassifier car_cascade;
    if (!car_cascade.load("cars.xml")) {
        std::cout << "Error in the cascade file loading!" << std::endl;
        throw runtime_error("Could not read cars.xml");
    }

    // load the image
    /*cv::Mat img = cv::imread(R"(C:\Users\aron7\Desktop\Mobileye BootCamp\Images\traffic_jam3.jpg)");
    if (img.empty()) {
        std::cout << "Error in the image loading!" << std::endl;
        return -1;
    }*/

    // convert to grayscale
    cv::Mat gray;
    cv::cvtColor(original_img, gray, cv::COLOR_BGR2GRAY);

    // identify cars in the image
    std::vector<cv::Rect> cars;
    car_cascade.detectMultiScale(gray, cars, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

    // draw rects around the cars
    for (size_t i = 0; i < cars.size(); i++)
    {
        cv::rectangle(original_img, cars[i].tl(), cars[i].br(), cv::Scalar(0, 0, 255), 2);
    }

    // show the image with the cars identifyed
    return cars.size();
}
