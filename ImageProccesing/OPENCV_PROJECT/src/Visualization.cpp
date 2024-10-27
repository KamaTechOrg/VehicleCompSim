#include "Visualization.h"
#include "Poligon.h"
#include "InputData.h"

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

//=================================================================================================
std::vector<std::pair<Poligon, ZONE_TYPES>>* Visualization::m_zonesPoligons = nullptr;
//=================================================================================================

void Visualization::drawPredictedObjects(cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects)
{
    // Draw bounding boxes for each tracked object
    for (const auto& [id, predicetedObject] : predictedObjects) {
        cv::Scalar color = predicetedObject.blinkInterval % 6 < 3 ?
            warningPriorityToColor.at(predicetedObject.warningPriority) : COLORS::BLACK;

        //SHADOW
        //if (int(predicetedObject.warningPriority) <= 2)
        //{
        //    const cv::Rect2d& rect = predicetedObject.bboxCurr;
        //    cv::Rect2d shadowRect(rect.x, rect.y, rect.width - 5, rect.height - 5);
        //    cv::rectangle(frame, shadowRect, cv::Scalar(144, 144, 255), 7);
        //}
        
        if (int(predicetedObject.warningPriority) <= 2)
        {
            cv::Mat roi = frame(predicetedObject.bboxCurr);

            cv::Mat coloredOverlay;
            roi.copyTo(coloredOverlay);
            cv::rectangle(coloredOverlay, cv::Point(0, 0), cv::Point(predicetedObject.bboxCurr.width, predicetedObject.bboxCurr.height), cv::Scalar(0, 0, 255), -1);
            cv::addWeighted(coloredOverlay, 0.2, roi, 1.0 - 0.2, 0, roi);
        }

        cv::rectangle(frame, predicetedObject.bboxCurr, color, 2);
    }
}

void Visualization::drawDistance(cv::Mat& frame, cv::Rect& rect, TrackedObject trackedObject)
{
    auto distance = trackedObject.distanceCurr;
    std::string distanceLabel = cv::format("%.2f m", distance);
    //cv::putText(frame, distanceLabel, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 2);
 
    // קביעת פרמטרי הפונט
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 2;

    // קבלת הגודל של הכיתוב
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(distanceLabel, fontFace, fontScale, thickness, &baseline);

    // חשב את מיקום הטקסט
    cv::Point textOrg(rect.x, rect.y);  // המיקום של הטקסט

    cv::Point rectStart(textOrg.x, textOrg.y - textSize.height);
    cv::Point rectEnd(textOrg.x + textSize.width, textOrg.y);

    // ציור רקע לבן מאחורי הכיתוב
    cv::rectangle(frame, rectStart, rectEnd, cv::Scalar(255, 255, 255), cv::FILLED);

    // ציור הכיתוב הצהוב על הרקע הלבן
    cv::putText(frame, distanceLabel, textOrg, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
}



void Visualization::drawRelativeSpeedWithDistances(cv::Mat& frame, const TrackedObject& predictedObject)
{
    auto& relativeSpeed = predictedObject.oldRelativSpeed;
    // הכנת המידע להצגה
    std::string speedLabel = cv::format("s: %.2f m/s", relativeSpeed);
    std::string currentDistanceLabel = cv::format("c: %.2f m", predictedObject.distanceCurr);
    std::string previousDistanceLabel = cv::format("p: %.2f m", predictedObject.distancePrev);

    // קביעת מיקום התצוגה על גבי הפריים
    int baseLine = int(predictedObject.bboxCurr.y) - 5;  // מעל המלבן
    int textHeight = 15; // גובה הטקסט בין שורה לשורה
    int padding = 5; // מרווח לרקע

    // מציאת גודל הטקסט עבור יצירת הרקע
    int baseline = 0;
    cv::Size textSizeSpeed = cv::getTextSize(speedLabel, cv::FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseline);
    cv::Size textSizeCurrent = cv::getTextSize(currentDistanceLabel, cv::FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseline);
    cv::Size textSizePrevious = cv::getTextSize(previousDistanceLabel, cv::FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseline);

    // ציור רקעים לבנים מאחורי כל שורת טקסט
    cv::rectangle(frame,
        cv::Point(int(predictedObject.bboxCurr.x) - padding, baseLine - textSizeSpeed.height - padding),
        cv::Point(int(predictedObject.bboxCurr.x) + textSizeSpeed.width + padding, baseLine + padding),
        cv::Scalar(255, 255, 255), cv::FILLED);

    cv::rectangle(frame,
        cv::Point(int(predictedObject.bboxCurr.x) - padding, baseLine - textHeight - textSizeCurrent.height - padding),
        cv::Point(int(predictedObject.bboxCurr.x) + textSizeCurrent.width + padding, baseLine - textHeight + padding),
        cv::Scalar(255, 255, 255), cv::FILLED);

    cv::rectangle(frame,
        cv::Point(int(predictedObject.bboxCurr.x) - padding, baseLine - 2 * textHeight - textSizePrevious.height - padding),
        cv::Point(int(predictedObject.bboxCurr.x) + textSizePrevious.width + padding, baseLine - 2 * textHeight + padding),
        cv::Scalar(255, 255, 255), cv::FILLED);

    // הצגת מהירות יחסית בטקסט שחור
    cv::putText(frame, speedLabel,
        cv::Point(int(predictedObject.bboxCurr.x), baseLine),
        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 2);

    // הצגת המרחק הנוכחי בטקסט שחור
    cv::putText(frame, currentDistanceLabel,
        cv::Point(int(predictedObject.bboxCurr.x), baseLine - textHeight),
        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 2);

    // הצגת המרחק הקודם בטקסט שחור
    cv::putText(frame, previousDistanceLabel,
        cv::Point(int(predictedObject.bboxCurr.x), baseLine - 2 * textHeight),
        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 2);
}

void Visualization::drawRelativeSpeed(cv::Mat& frame, const TrackedObject& predictedObject, float relativeSpeed)
{
    std::string speedLabel = cv::format("%.2f m/s", relativeSpeed);
    cv::putText(frame, speedLabel, cv::Point(int(predictedObject.bboxCurr.x), int(predictedObject.bboxCurr.y)), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 2);
}

void Visualization::drawCategory(cv::Mat& frame, std::unordered_map<int, TrackedObject>& predictedObjects)
{
}
//==================================================================
void Visualization::drawZones(cv::Mat& frame)
{
    for (auto& [poligon, zoneType] : *m_zonesPoligons)
    {
        const std::vector<cv::Point>& poligonVec = poligon.getPoligon();
        const cv::Point* points[1] = { &poligonVec[0]};
        int num_points = static_cast<int>(poligonVec.size());
        cv::polylines(frame, points, &num_points, 1, true, zoneTypeToColor.at(zoneType), 3);
    }
}

//void Visualization::drawZones(cv::Mat& frame)
//{
//    // cars hood
//    std::vector<cv::Point> trapezoid_points;
//    trapezoid_points.push_back(TRAPEZOID::CAR_HOOD::DL);// WS
//    trapezoid_points.push_back(TRAPEZOID::CAR_HOOD::DR);// ES
//    trapezoid_points.push_back(TRAPEZOID::CAR_HOOD::UR);// EN
//    trapezoid_points.push_back(TRAPEZOID::CAR_HOOD::UL);// WN
//
//    const cv::Point* points[1] = { &trapezoid_points[0] };
//    int num_points = static_cast<int>(trapezoid_points.size());
//    cv::polylines(frame, points, &num_points, 1, true, COLORS::BLACK, 3);
//
//    // CRIITICAL_ZONE_1
//    trapezoid_points.clear();
//    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_1::DL);// WS
//    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_1::DR);// ES
//    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_1::UR);// EN
//    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_1::UL);// WN
//
//    points[0] = { &trapezoid_points[0] };
//    num_points = static_cast<int>(trapezoid_points.size());
//    cv::polylines(frame, points, &num_points, 1, true, zoneTypeToColor.at(ZONE_TYPES::CRITICAL_ZONE_1), 3);
//
//    //CRITICAL_ZONE_2
//    trapezoid_points.clear();
//    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_2::DL);// WS
//    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_2::DR);// ES
//    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_2::UR);// EN
//    trapezoid_points.push_back(TRAPEZOID::CRITICAL_ZONE_2::UL);// WN
//
//    points[0] = { &trapezoid_points[0] };
//    num_points = static_cast<int>(trapezoid_points.size());
//    cv::polylines(frame, points, &num_points, 1, true, zoneTypeToColor.at(ZONE_TYPES::CRITICAL_ZONE_2), 3);
//
//    //TRACKED_ZONE
//    trapezoid_points.clear();
//    trapezoid_points.push_back(TRAPEZOID::TRACKED_ZONE::DL);// WS
//    trapezoid_points.push_back(TRAPEZOID::TRACKED_ZONE::DR);// ES
//    trapezoid_points.push_back(TRAPEZOID::TRACKED_ZONE::UR);// EN
//    trapezoid_points.push_back(TRAPEZOID::TRACKED_ZONE::UL);// WN
//
//    points[0] = { &trapezoid_points[0] };
//    num_points = static_cast<int>(trapezoid_points.size());
//    cv::polylines(frame, points, &num_points, 1, true, zoneTypeToColor.at(ZONE_TYPES::TRACKED_ZONE), 3);
//}
void Visualization::drawWarningDescription(cv::Mat& frame, const TrackedObject& trackedObject)
{
	int fontFace = cv::FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.5;
	int thickness = 2;

	int baseline = 0;
	cv::Size textSize = cv::getTextSize(trackedObject.warningDescription, fontFace, fontScale, thickness, &baseline);

	cv::Point textOrg(int(trackedObject.bboxCurr.x), int(trackedObject.bboxCurr.y));  // המיקום של הטקסט

	cv::Point rectStart(textOrg.x, textOrg.y - textSize.height);
	cv::Point rectEnd(textOrg.x + textSize.width, textOrg.y);

	cv::rectangle(frame, rectStart, rectEnd, cv::Scalar(255, 255, 255), cv::FILLED);
	cv::putText(frame, trackedObject.warningDescription, textOrg, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
}

void Visualization::setCurrentVideoPoligons(const std::string& videoName)
{
    m_zonesPoligons = &(InputData::getInstance().getZonesPoligon(videoName));
}
