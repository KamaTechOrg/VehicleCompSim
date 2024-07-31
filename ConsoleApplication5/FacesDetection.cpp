


//
//#include "open_cv.h"
//using namespace cv;
//
//
//
//
////int main(int argc, char** argv) {
//    
//    //if (argc < 2) {
//    //    std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
//    //    return -1;
//    //}
//int main() {
//    std::string imagePath = R"(C:\Users\dmaro\Downloads\people.jpg)";
//    std::string cascaderFile = "haarcascade_frontalface_default.xml";
//
//    cv::CascadeClassifier faceCascade;
//    if (!faceCascade.load(cascaderFile)) {
//        std::cerr << "Error loading haarcascade_frontalface_default.xml" << std::endl;
//        return -1;
//    }
//
//    cv::Mat image = cv::imread(imagePath);
//    if (image.empty()) {
//        std::cerr << "Error loading image: " << imagePath << std::endl;
//        return -1;
//    }
//
//    cv::Mat gray;
//    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
//
//    std::vector<cv::Rect> faces;
//    faceCascade.detectMultiScale(gray, faces, 1.1, 5, 0, cv::Size(30, 30));
//
//    std::cout << "Found " << faces.size() << " faces!" << std::endl;
//
//    for (size_t i = 0; i < faces.size(); ++i) {
//        cv::rectangle(image, faces[i], cv::Scalar(0, 255, 0), 2);
//    }
//
//    cv::imshow("Faces found", image);
//    cv::waitKey(0);
//
//    return 0;
//}