

#include "Manager.h"
#include "runLaneDetection.h"


int main() {
    runLaneDetection();
    Manager().runTracking();
    return 0;
}



//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <utility>
//#include <optional>
//
//
//// פונקציה שמחשבת את נקודת החיתוך בין שני קווים, כל קו מיוצג על ידי שני cv::Point
//std::optional<cv::Point> findIntersection(cv::Mat& image, const cv::Point& p1, const cv::Point& p2,
//    const cv::Point& q1, const cv::Point& q2) {
//    // חישוב ההפרשים
//    int a1 = p2.y - p1.y; // y2 - y1
//    int b1 = p1.x - p2.x; // x1 - x2
//    int c1 = a1 * p1.x + b1 * p1.y;
//
//    int a2 = q2.y - q1.y; // y2 - y1
//    int b2 = q1.x - q2.x; // x1 - x2
//    int c2 = a2 * q1.x + b2 * q1.y;
//
//    // חישוב הדטרמיננטה
//    int determinant = a1 * b2 - a2 * b1;
//
//    // אם הדטרמיננטה היא 0, הקווים מקבילים ואין חיתוך
//    if (determinant == 0) {
//        return std::nullopt; // אין חיתוך
//    }
//    else {
//        // חישוב נקודת החיתוך
//        int x = (b2 * c1 - b1 * c2) / determinant;
//        int y = (a1 * c2 - a2 * c1) / determinant;
//        //בדיקה אם נקודת החיתוך נמצאת מחוץ לגבולות התמונה
//        if (x < 0 || x > image.cols || y < 0 || y > image.rows) {
//			return std::nullopt; // אין חיתוך
//		}
//
//        return cv::Point(x, y);
//    }
//}
//
//std::pair<std::pair<cv::Point, cv::Point>, std::pair<cv::Point, cv::Point>> getMovementDirection(cv::Mat& image, const cv::Rect& prevRect, const cv::Rect& currRect)
//{
//    std::pair<cv::Point, cv::Point> firstVector;
//    std::pair<cv::Point, cv::Point> secondVector;
//    // הבדלים בציר ה-X ובציר ה-Y
//    cv::Point prevTL = prevRect.tl();
//    cv::Point currTL = currRect.tl();
//    cv::Point prevTR = cv::Point(prevRect.x + prevRect.width, prevRect.y);
//    cv::Point currTR = cv::Point(currRect.x + currRect.width, currRect.y);
//    cv::Point prevBL = cv::Point(prevRect.x, prevRect.y + prevRect.height);
//    cv::Point currBL = cv::Point(currRect.x, currRect.y + currRect.height);
//    cv::Point prevBR = prevRect.br();
//    cv::Point currBR = currRect.br();
//
//    int deltaX = currRect.x - prevRect.x;
//    int deltaY = currRect.y - prevRect.y;
//
//    if ((deltaX > 0 && deltaY > 0) || (deltaX < 0 && deltaY < 0)) {
//        firstVector = std::make_pair(prevTR, currTR);
//        secondVector = std::make_pair(prevBL, currBL);
//    }
//    else {
//        firstVector = std::make_pair(prevTL, currTL);
//        secondVector = std::make_pair(prevBR, currBR);
//    }
//    //
//
//    std::pair<cv::Point, cv::Point> imageUpperBorder = std::make_pair(cv::Point(0, 0), cv::Point(image.cols, 0));
//	std::pair<cv::Point, cv::Point> imageLowerBorder = std::make_pair(cv::Point(0, image.rows), cv::Point(image.cols, image.rows));
//	std::pair<cv::Point, cv::Point> imageLeftBorder = std::make_pair(cv::Point(0, 0), cv::Point(0, image.rows));
//	std::pair<cv::Point, cv::Point> imageRightBorder = std::make_pair(cv::Point(image.cols, 0), cv::Point(image.cols, image.rows));
//
//	
//
//	// נקודות החיתוך עם גבולות התמונה
//   if (deltaX > 0 && deltaY > 0) {
//       // חישוב נקודות החיתוך עם גבול התמונה התחתון
//       auto firstIntersection = findIntersection(image,firstVector.first, firstVector.second, imageLowerBorder.first, imageLowerBorder.second);
//       auto secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageLowerBorder.first, imageLowerBorder.second);
//
//       // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
//       if (firstIntersection.has_value()) {
//           firstVector.second = firstIntersection.value();
//       }
//       else {
//           // חישוב נקודת החיתוך עם גבול התמונה  הימני
//           firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageRightBorder.first, imageRightBorder.second);
//           if (firstIntersection.has_value()) {
//               firstVector.second = firstIntersection.value();
//           }
//       }
//       if (secondIntersection.has_value()) {
//           secondVector.second = secondIntersection.value();
//       }
//       else {
//           // חישוב נקודת החיתוך עם גבול התמונה  הימני
//           secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageRightBorder.first, imageRightBorder.second);
//           if (secondIntersection.has_value()) {
//               secondVector.second = secondIntersection.value();
//           }
//       }
//
//   }
//   else if (deltaX < 0 && deltaY > 0) {
//       // חישוב נקודות החיתוך עם גבול התמונה התחתון
//       auto firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageLowerBorder.first, imageLowerBorder.second);
//       auto secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageLowerBorder.first, imageLowerBorder.second);
//
//       // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
//       if (firstIntersection.has_value()) {
//           firstVector.second = firstIntersection.value();
//       }
//       else {
//           // חישוב נקודת החיתוך עם גבול התמונה  השמאלי
//           firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageLeftBorder.first, imageLeftBorder.second);
//           if (firstIntersection.has_value()) {
//               firstVector.second = firstIntersection.value();
//           }
//       }
//       if (secondIntersection.has_value()) {
//           secondVector.second = secondIntersection.value();
//       }
//       else {
//           // חישוב נקודת החיתוך עם גבול התמונה  השמאלי
//           secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageLeftBorder.first, imageLeftBorder.second);
//           if (secondIntersection.has_value()) {
//               secondVector.second = secondIntersection.value();
//           }
//       }
//   }
//   else if (deltaX > 0 && deltaY < 0) {
//       // חישוב נקודות החיתוך עם גבול התמונה העליון
//       auto firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageUpperBorder.first, imageUpperBorder.second);
//       auto secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageUpperBorder.first, imageUpperBorder.second);
//
//       // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
//       if (firstIntersection.has_value()) {
//           firstVector.second = firstIntersection.value();
//       }
//       else {
//           // חישוב נקודת החיתוך עם גבול התמונה  הימני
//           firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageRightBorder.first, imageRightBorder.second);
//           if (firstIntersection.has_value()) {
//               firstVector.second = firstIntersection.value();
//           }
//       }
//       if (secondIntersection.has_value()) {
//           secondVector.second = secondIntersection.value();
//       }
//       else {
//           // חישוב נקודת החיתוך עם גבול התמונה  הימני
//           secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageRightBorder.first, imageRightBorder.second);
//           if (secondIntersection.has_value()) {
//               secondVector.second = secondIntersection.value();
//           }
//       }
//   }
//   else if (deltaX < 0 && deltaY < 0) {
//       // חישוב נקודות החיתוך עם גבול התמונה העליון
//       auto firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageUpperBorder.first, imageUpperBorder.second);
//       auto secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageUpperBorder.first, imageUpperBorder.second);
//
//       // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
//       if (firstIntersection.has_value()) {
//           firstVector.second = firstIntersection.value();
//       }
//       else {
//           // חישוב נקודת החיתוך עם גבול התמונה  השמאלי
//           firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageLeftBorder.first, imageLeftBorder.second);
//           if (firstIntersection.has_value()) {
//               firstVector.second = firstIntersection.value();
//           }
//       }
//       if (secondIntersection.has_value()) {
//           secondVector.second = secondIntersection.value();
//       }
//       else {
//           // חישוב נקודת החיתוך עם גבול התמונה  השמאלי
//           secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageLeftBorder.first, imageLeftBorder.second);
//           if (secondIntersection.has_value()) {
//               secondVector.second = secondIntersection.value();
//           }
//       }
//   }
//   else if (deltaX == 0 && deltaY < 0) {
//       // חישוב נקודת החיתוך עם גבול התמונה העליון
//       auto firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageUpperBorder.first, imageUpperBorder.second);
//       auto secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageUpperBorder.first, imageUpperBorder.second);
//
//       // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
//       if (firstIntersection.has_value()) {
//           firstVector.second = firstIntersection.value();
//       }
//       if (secondIntersection.has_value()) {
//           secondVector.second = secondIntersection.value();
//       }
//   }
//   else if (deltaX == 0 && deltaY > 0) {
//       // חישוב נקודת החיתוך עם גבול התמונה התחתון
//       auto firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageLowerBorder.first, imageLowerBorder.second);
//       auto secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageLowerBorder.first, imageLowerBorder.second);
//
//       // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
//       if (firstIntersection.has_value()) {
//           firstVector.second = firstIntersection.value();
//       }
//       if (secondIntersection.has_value()) {
//           secondVector.second = secondIntersection.value();
//       }
//   }
//   else if (deltaX > 0 && deltaY == 0) {
//       
//       // חישוב נקודת החיתוך עם גבול התמונה השמאלי
//       auto firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageLeftBorder.first, imageLeftBorder.second);
//       auto secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageLeftBorder.first, imageLeftBorder.second);
//
//       // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
//       if (firstIntersection.has_value()) {
//           firstVector.second = firstIntersection.value();
//       }
//       if (secondIntersection.has_value()) {
//           secondVector.second = secondIntersection.value();
//       }
//   }
//   else if (deltaX < 0 && deltaY == 0) {
//       // חישוב נקודת החיתוך עם גבול התמונה הימני
//       auto firstIntersection = findIntersection(image, firstVector.first, firstVector.second, imageRightBorder.first, imageRightBorder.second);
//       auto secondIntersection = findIntersection(image, secondVector.first, secondVector.second, imageRightBorder.first, imageRightBorder.second);
//
//       // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
//       if (firstIntersection.has_value()) {
//           firstVector.second = firstIntersection.value();
//       }
//       if (secondIntersection.has_value()) {
//           secondVector.second = secondIntersection.value();
//       }
//   } 
//
//   return std::make_pair(firstVector, secondVector);
//}
//
//
//// פונקציה לציור הווקטורים והמלבן הנוכחי
//void drawMovementVectorsAndRect(
//    cv::Mat& image,
//    const cv::Rect& currRect,
//    const std::pair<std::pair<cv::Point, cv::Point>, std::pair<cv::Point, cv::Point>>& movementVectors)
//{
//    // ציור הוקטורים (קווים)
//    cv::line(image, movementVectors.first.first, movementVectors.first.second, cv::Scalar(0, 255, 0), 2); // ירוק
//    cv::line(image, movementVectors.second.first, movementVectors.second.second, cv::Scalar(0, 255, 0), 2); // ירוק
//
//    // ציור המלבן הנוכחי בצבע כחול
//    cv::rectangle(image, currRect, cv::Scalar(255, 0, 0), 2); // כחול
//}
//
//
//int main()
//{
//    std::string image_path = R"(C:\Users\dmaro\source\repos\ConsoleApplication6 (3) - Copy\ConsoleApplication6\ConsoleApplication6\first_frame.png)";
//    // טוענים את התמונה (נניח שתמונה נמצאת בנתיב הנכון)
//    cv::Mat image = cv::imread(image_path);
//    if (image.empty()) {
//        std::cerr << "Failed to load image" << std::endl;
//        return -1;
//    }
//
//    // מיקום המלבן בפריים הקודם והנוכחי
//    cv::Rect prevRect(500, 400, 50, 50);  // מיקום בפריים הקודם
//    cv::Rect currRect(550, 350, 50, 50);  // מיקום בפריים הנוכחי
//    auto result_pair = getMovementDirection(image, prevRect, currRect);
//    
//    // ציור הווקטורים והמלבן על גבי התמונה
//    drawMovementVectorsAndRect(image, currRect, result_pair);
//
//    // הצגת התמונה עם המלבן והווקטורים
//    cv::imshow("Object Movement", image);
//    cv::waitKey(0);
//
//    return 0;
//}






//
//#include <opencv2/opencv.hpp>
//#include <opencv2/tracking.hpp>
//
//using namespace cv;
//using namespace std;
//
//// Convert to string
//#define SSTR(x) std::to_string(x)
//
//int main()
//{
//    // List of tracker types
//    string trackerTypes[8] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT" };
//
//    // Create a tracker
//    string trackerType = trackerTypes[2]; // Example: "KCF"
//
//    Ptr<Tracker> tracker;
//
//    if (trackerType == "BOOSTING"){}
//        //tracker = TrackerBoosting::create();
//    else if (trackerType == "MIL")
//        tracker = TrackerMIL::create();
//    else if (trackerType == "KCF")
//        tracker = TrackerKCF::create();
//    else if (trackerType == "TLD"){}
//       // tracker = TrackerTLD::create();
//    else if (trackerType == "MEDIANFLOW"){}
//       // tracker = TrackerMedianFlow::create();
//    else if (trackerType == "GOTURN")
//        tracker = TrackerGOTURN::create();
//    else if (trackerType == "MOSSE"){}
//        //tracker = TrackerMOSSE::create();
//    else if (trackerType == "CSRT")
//        tracker = TrackerCSRT::create();
//
//    // Read video
//    std::string video_path = R"(C:\Users\dmaro\Downloads\Race.mp4)";
//    VideoCapture video(video_path);
//
//    // Exit if video is not opened
//    if (!video.isOpened())
//    {
//        cout << "Could not read video file" << endl;
//        return 1;
//    }
//
//    // Read first frame 
//    Mat frame;
//    bool ok = video.read(frame);
//
//    // Define initial bounding box 
//    Rect bbox(287, 23, 86, 320);
//
//    // Uncomment the line below to select a different bounding box 
//    // bbox = selectROI(frame, false); 
//    // Display bounding box. 
//    rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
//
//    imshow("Tracking", frame);
//    tracker->init(frame, bbox);
//
//    while (video.read(frame))
//    {
//        // Start timer
//        double timer = (double)getTickCount();
//
//        // Update the tracking result
//        bool ok = tracker->update(frame, bbox);
//
//        // Calculate Frames per second (FPS)
//        float fps = getTickFrequency() / ((double)getTickCount() - timer);
//
//        if (ok)
//        {
//            // Tracking success : Draw the tracked object
//            rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
//        }
//        else
//        {
//            // Tracking failure detected.
//            putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
//        }
//
//        // Display tracker type on frame
//        putText(frame, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
//
//        // Display FPS on frame
//        putText(frame, "FPS : " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
//
//        // Display frame.
//        imshow("Tracking", frame);
//
//        // Exit if ESC pressed.
//        int k = waitKey(1);
//        if (k == 27)
//        {
//            break;
//        }
//
//    }
//
//    return 0;
//}

// 
//
//
//
//
////#include <iostream>
////#include <opencv2/opencv.hpp>
////
////int main()
////{
////    std::string image_path = R"(C:\Users\dmaro\Downloads\cars2.jpeg)";
////
////    cv::Mat image = cv::imread(image_path);
////
////    if (image.empty())
////    {
////        std::cout << "Could not open or find the image!" << std::endl;
////        return -1;
////    }
////
////    cv::imshow("Display Image", image);
////    cv::waitKey(0);
////
////    return 0;
////}
