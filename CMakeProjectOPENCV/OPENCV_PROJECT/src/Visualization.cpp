#include "Visualization.h"
#include "Poligon.h"


cv::Scalar pink = cv::Scalar(203, 192, 255);
cv::Scalar blue = cv::Scalar(255, 0, 0);
cv::Scalar green = cv::Scalar(0, 255, 0);
cv::Scalar yellow = cv::Scalar(0, 255, 255);
cv::Scalar red = cv::Scalar(0, 0, 255);
cv::Scalar white = cv::Scalar(255, 255, 255);
cv::Scalar black = cv::Scalar(0, 0, 0);
cv::Scalar lightBlue = cv::Scalar(255, 228, 173);
cv::Scalar purple = cv::Scalar(128, 0, 128);
cv::Scalar orange = cv::Scalar(0, 165, 255);


void Visualization::drawPredictedObjects(cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects)
{
    // Draw bounding boxes for each tracked object
    for (const auto& [id, predicetedObject] : predictedObjects) {
        const auto& bbox = predicetedObject.bboxCurr;
        cv::Scalar color;
        if (predicetedObject.warningPriority == WarningPriority::Safe || predicetedObject.warningPriority == WarningPriority::Low) {
            color = zoneTypeToColor.at(predicetedObject.zoneTypeIntersect);
        }
        else {
            color = warningPriorityToColor.at(predicetedObject.warningPriority);
        }
        cv::rectangle(frame, bbox, color, 2);
    }
}
//    std::vector<cv::Point> pointsOfFirst;
//    pointsOfFirst.push_back(cv::Point(111, 590));   // ���� ������ ������
//    pointsOfFirst.push_back(cv::Point(880, 590));  // ���� ����� ������
//    pointsOfFirst.push_back(cv::Point(735, 450));          // ���� ����� ������
//    pointsOfFirst.push_back(cv::Point(390, 450));          // ���� ������ ������
//    Poligon firstPoligon(pointsOfFirst);
//    
//    std::vector<cv::Point> pointsOfSecond;
//    pointsOfSecond.push_back(cv::Point(390, 450));   // ���� ������ ������
//    pointsOfSecond.push_back(cv::Point(735, 450));  // ���� ����� ������
//    pointsOfSecond.push_back(cv::Point(650, 370));          // ���� ����� ������
//    pointsOfSecond.push_back(cv::Point(540, 370));		  // ���� ������ ������
//    Poligon secondPoligon(pointsOfSecond);
//
//    // Draw bounding boxes for each tracked object
//    for (const auto& [id, predicetedObject] : predictedObjects) {
//        auto& bbox = predicetedObject.bboxCurr;
//
//        cv::Scalar color;
//
//        // Choose a color based on category
//        if (firstPoligon.doesRectIntersectPolygon(bbox)) {
//            color = orange;
//        }
//        else if(secondPoligon.doesRectIntersectPolygon(bbox)){
//            color = yellow;
//		}
//        //else if (toColor.find(id) != toColor.end())
//        //{
//        //    color = red; // Red for warning
//        //}
//        else if (predicetedObject.category == "car") {
//            color = blue;
//        }
//        else if (predicetedObject.category == "person") {
//            color = lightBlue;
//        }
//        else {
//            color = black;
//        }
//
//        // Draw the bounding box
//        cv::rectangle(frame, bbox, color, 2);
//    }
//}

void Visualization::drawDistance(cv::Mat& frame, cv::Rect& rect, TrackedObject trackedObject)
{
    auto distance = trackedObject.distanceCurr;
    std::string distanceLabel = cv::format("%.2f m", distance);
    //cv::putText(frame, distanceLabel, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 2);
 
    // ����� ������ �����
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 2;

    // ���� ����� �� ������
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(distanceLabel, fontFace, fontScale, thickness, &baseline);

    // ��� �� ����� �����
    cv::Point textOrg(rect.x, rect.y);  // ������ �� �����

    cv::Point rectStart(textOrg.x, textOrg.y - textSize.height);
    cv::Point rectEnd(textOrg.x + textSize.width, textOrg.y);

    // ���� ��� ��� ������ ������
    cv::rectangle(frame, rectStart, rectEnd, cv::Scalar(255, 255, 255), cv::FILLED);

    // ���� ������ ����� �� ���� ����
    cv::putText(frame, distanceLabel, textOrg, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
}



void Visualization::drawRelativeSpeedWithDistances(cv::Mat& frame, const TrackedObject& predictedObject)
{
    auto& relativeSpeed = predictedObject.oldRelativSpeed;
    // ���� ����� �����
    std::string speedLabel = cv::format("s: %.2f m/s", relativeSpeed);
    std::string currentDistanceLabel = cv::format("c: %.2f m", predictedObject.distanceCurr);
    std::string previousDistanceLabel = cv::format("p: %.2f m", predictedObject.distancePrev);

    // ����� ����� ������ �� ��� ������
    int baseLine = predictedObject.bboxCurr.y - 5;  // ��� �����
    int textHeight = 15; // ���� ����� ��� ���� �����
    int padding = 5; // ����� ����

    // ����� ���� ����� ���� ����� ����
    int baseline = 0;
    cv::Size textSizeSpeed = cv::getTextSize(speedLabel, cv::FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseline);
    cv::Size textSizeCurrent = cv::getTextSize(currentDistanceLabel, cv::FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseline);
    cv::Size textSizePrevious = cv::getTextSize(previousDistanceLabel, cv::FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseline);

    // ���� ����� ����� ������ �� ���� ����
    cv::rectangle(frame,
        cv::Point(predictedObject.bboxCurr.x - padding, baseLine - textSizeSpeed.height - padding),
        cv::Point(predictedObject.bboxCurr.x + textSizeSpeed.width + padding, baseLine + padding),
        cv::Scalar(255, 255, 255), cv::FILLED);

    cv::rectangle(frame,
        cv::Point(predictedObject.bboxCurr.x - padding, baseLine - textHeight - textSizeCurrent.height - padding),
        cv::Point(predictedObject.bboxCurr.x + textSizeCurrent.width + padding, baseLine - textHeight + padding),
        cv::Scalar(255, 255, 255), cv::FILLED);

    cv::rectangle(frame,
        cv::Point(predictedObject.bboxCurr.x - padding, baseLine - 2 * textHeight - textSizePrevious.height - padding),
        cv::Point(predictedObject.bboxCurr.x + textSizePrevious.width + padding, baseLine - 2 * textHeight + padding),
        cv::Scalar(255, 255, 255), cv::FILLED);

    // ���� ������ ����� ����� ����
    cv::putText(frame, speedLabel,
        cv::Point(predictedObject.bboxCurr.x, baseLine),
        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 2);

    // ���� ����� ������ ����� ����
    cv::putText(frame, currentDistanceLabel,
        cv::Point(predictedObject.bboxCurr.x, baseLine - textHeight),
        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 2);

    // ���� ����� ����� ����� ����
    cv::putText(frame, previousDistanceLabel,
        cv::Point(predictedObject.bboxCurr.x, baseLine - 2 * textHeight),
        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 2);
}

void Visualization::drawRelativeSpeed(cv::Mat& frame, const TrackedObject& predictedObject, float relativeSpeed)
{
    std::string speedLabel = cv::format("%.2f m/s", relativeSpeed);
    cv::putText(frame, speedLabel, cv::Point(predictedObject.bboxCurr.x, predictedObject.bboxCurr.y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 2);
}

void Visualization::drawCategory(cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects)
{
}

void Visualization::drawCarsHoodTarpezoid(cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects)
{
    // ���� ���� �� ���� �����:
    std::vector<cv::Point> trapezoid_points;
    trapezoid_points.push_back(cv::Point(0, frame.rows));   // ���� ������ ������
    trapezoid_points.push_back(cv::Point(frame.cols, frame.rows));  // ���� ����� ������
    trapezoid_points.push_back(cv::Point(1277, 590));          // ���� ����� ������
    trapezoid_points.push_back(cv::Point(3, 590));          // ���� ������ ������

    // ���� �� �����
    const cv::Point* points[1] = { &trapezoid_points[0] };
    int num_points = static_cast<int>(trapezoid_points.size());
    cv::polylines(frame, points, &num_points, 1, true, cv::Scalar(255, 0, 0), 3);
    
    //........���� ���� 1:
    // ���� ����������� �����
    trapezoid_points.clear();
    trapezoid_points.push_back(cv::Point(111, 590));   // ���� ������ ������
    trapezoid_points.push_back(cv::Point(880, 590));  // ���� ����� ������
    trapezoid_points.push_back(cv::Point(735, 450));          // ���� ����� ������
    trapezoid_points.push_back(cv::Point(390, 450));          // ���� ������ ������

    // ���� �� �����
    points[0] = { &trapezoid_points[0] };
    num_points = static_cast<int>(trapezoid_points.size());
    cv::polylines(frame, points, &num_points, 1, true, orange, 3);

    //.......2��� ���� :
// ���� ����������� �����
    trapezoid_points.clear();
    trapezoid_points.push_back(cv::Point(390, 450));   // ���� ������ ������
    trapezoid_points.push_back(cv::Point(735, 450));  // ���� ����� ������
    trapezoid_points.push_back(cv::Point(650, 370));          // ���� ����� ������
    trapezoid_points.push_back(cv::Point(540, 370));          // ���� ������ ������

    // ���� �� �����
    points[0] = { &trapezoid_points[0] };
    num_points = static_cast<int>(trapezoid_points.size());
    cv::polylines(frame, points, &num_points, 1, true, yellow, 3);
}


//==================================================================
void Visualization::drawZones(cv::Mat& frame)
{
    // cars hood
    std::vector<cv::Point> trapezoid_points;
    trapezoid_points.push_back(TRAPEZOID::CAR_HOOD::DL);// WS
    trapezoid_points.push_back(TRAPEZOID::CAR_HOOD::DR);// ES
    trapezoid_points.push_back(TRAPEZOID::CAR_HOOD::UR);// EN
    trapezoid_points.push_back(TRAPEZOID::CAR_HOOD::UL);// WN

    const cv::Point* points[1] = { &trapezoid_points[0] };
    int num_points = static_cast<int>(trapezoid_points.size());
    cv::polylines(frame, points, &num_points, 1, true, COLORS::BLACK, 3);

    // CRIITICAL_ZONE_1
    trapezoid_points.clear();
    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_1::DL);// WS
    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_1::DR);// ES
    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_1::UR);// EN
    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_1::UL);// WN

    points[0] = { &trapezoid_points[0] };
    num_points = static_cast<int>(trapezoid_points.size());
    cv::polylines(frame, points, &num_points, 1, true, zoneTypeToColor.at(ZONE_TYPES::CRITICAL_ZONE_1), 3);

    //CRITICAL_ZONE_2
    trapezoid_points.clear();
    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_2::DL);// WS
    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_2::DR);// ES
    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_2::UR);// EN
    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_2::UL);// WN

    points[0] = { &trapezoid_points[0] };
    num_points = static_cast<int>(trapezoid_points.size());
    cv::polylines(frame, points, &num_points, 1, true, zoneTypeToColor.at(ZONE_TYPES::CRITICAL_ZONE_2), 3);

    //TRACKED_ZONE
    trapezoid_points.clear();
    trapezoid_points.push_back(TRAPEZOID::TRACKED_ZONE::DL);// WS
    trapezoid_points.push_back(TRAPEZOID::TRACKED_ZONE::DR);// ES
    trapezoid_points.push_back(TRAPEZOID::TRACKED_ZONE::UR);// EN
    trapezoid_points.push_back(TRAPEZOID::TRACKED_ZONE::UL);// WN

    points[0] = { &trapezoid_points[0] };
    num_points = static_cast<int>(trapezoid_points.size());
    cv::polylines(frame, points, &num_points, 1, true, zoneTypeToColor.at(ZONE_TYPES::TRACKED_ZONE), 3);
}

void Visualization::drawWarningDescription(cv::Mat& frame, const TrackedObject& trackedObject)
{
    // ����� ������ �����
	int fontFace = cv::FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.5;
	int thickness = 2;

	// ���� ����� �� ������
	int baseline = 0;
	cv::Size textSize = cv::getTextSize(trackedObject.warningDescription, fontFace, fontScale, thickness, &baseline);

	// ��� �� ����� �����
	cv::Point textOrg(trackedObject.bboxCurr.x, trackedObject.bboxCurr.y);  // ������ �� �����

	cv::Point rectStart(textOrg.x, textOrg.y - textSize.height);
	cv::Point rectEnd(textOrg.x + textSize.width, textOrg.y);

	// ���� ��� ��� ������ ������
	cv::rectangle(frame, rectStart, rectEnd, cv::Scalar(255, 255, 255), cv::FILLED);

	// ���� ������ ����� �� ���� ����
	cv::putText(frame, trackedObject.warningDescription, textOrg, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
}
