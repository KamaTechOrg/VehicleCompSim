#include "NewPrediction.h"
#include <optional>


// פונקציה שמחשבת את נקודת החיתוך בין שני קווים, כל קו מיוצג על ידי שני cv::Point
std::optional<cv::Point> findIntersection(const std::pair<int, int>& imageBorders, const cv::Point& p1, const cv::Point& p2,
    const cv::Point& q1, const cv::Point& q2) {
    // חישוב ההפרשים
    int a1 = p2.y - p1.y; // y2 - y1
    int b1 = p1.x - p2.x; // x1 - x2
    int c1 = a1 * p1.x + b1 * p1.y;

    int a2 = q2.y - q1.y; // y2 - y1
    int b2 = q1.x - q2.x; // x1 - x2
    int c2 = a2 * q1.x + b2 * q1.y;

    // חישוב הדטרמיננטה
    int determinant = a1 * b2 - a2 * b1;

    // אם הדטרמיננטה היא 0, הקווים מקבילים ואין חיתוך
    if (determinant == 0) {
        return std::nullopt; // אין חיתוך
    }
    else {
        auto& cols = imageBorders.first;
        auto& rows = imageBorders.second;
        // חישוב נקודת החיתוך
        int x = (b2 * c1 - b1 * c2) / determinant;
        int y = (a1 * c2 - a2 * c1) / determinant;
        //בדיקה אם נקודת החיתוך נמצאת מחוץ לגבולות התמונה
        if (x < 0 || x > cols || y < 0 || y > rows) {
            return std::nullopt; // אין חיתוך
        }

        return cv::Point(x, y);
    }
}

std::pair<std::pair<cv::Point, cv::Point>, std::pair<cv::Point, cv::Point>> getMovementDirection(const cv::Rect& prevRect, const cv::Rect& currRect)
{
    const std::pair<int, int>& imageBorders = IMAGE_BORDERS;

    std::pair<cv::Point, cv::Point> firstVector;
    std::pair<cv::Point, cv::Point> secondVector;
    // הבדלים בציר ה-X ובציר ה-Y
    cv::Point prevTL = prevRect.tl();
    cv::Point currTL = currRect.tl();
    cv::Point prevTR = cv::Point(prevRect.x + prevRect.width, prevRect.y);
    cv::Point currTR = cv::Point(currRect.x + currRect.width, currRect.y);
    cv::Point prevBL = cv::Point(prevRect.x, prevRect.y + prevRect.height);
    cv::Point currBL = cv::Point(currRect.x, currRect.y + currRect.height);
    cv::Point prevBR = prevRect.br();
    cv::Point currBR = currRect.br();

    int deltaX = currRect.x - prevRect.x;
    int deltaY = currRect.y - prevRect.y;

    if ((deltaX > 0 && deltaY > 0) || (deltaX < 0 && deltaY < 0)) {
        firstVector = std::make_pair(prevTR, currTR);
        secondVector = std::make_pair(prevBL, currBL);
    }
    else {
        firstVector = std::make_pair(prevTL, currTL);
        secondVector = std::make_pair(prevBR, currBR);
    }
    //
    auto& cols = imageBorders.first;
    auto& rows = imageBorders.second;
    std::pair<cv::Point, cv::Point> imageUpperBorder = std::make_pair(cv::Point(0, 0), cv::Point(cols, 0));
    std::pair<cv::Point, cv::Point> imageLowerBorder = std::make_pair(cv::Point(0, rows), cv::Point(cols, rows));
    std::pair<cv::Point, cv::Point> imageLeftBorder = std::make_pair(cv::Point(0, 0), cv::Point(0, rows));
    std::pair<cv::Point, cv::Point> imageRightBorder = std::make_pair(cv::Point(cols, 0), cv::Point(cols, rows));



    // נקודות החיתוך עם גבולות התמונה
    if (deltaX > 0 && deltaY > 0) {
        // חישוב נקודות החיתוך עם גבול התמונה התחתון
        auto firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageLowerBorder.first, imageLowerBorder.second);
        auto secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageLowerBorder.first, imageLowerBorder.second);

        // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
        if (firstIntersection.has_value()) {
            firstVector.second = firstIntersection.value();
        }
        else {
            // חישוב נקודת החיתוך עם גבול התמונה  הימני
            firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageRightBorder.first, imageRightBorder.second);
            if (firstIntersection.has_value()) {
                firstVector.second = firstIntersection.value();
            }
        }
        if (secondIntersection.has_value()) {
            secondVector.second = secondIntersection.value();
        }
        else {
            // חישוב נקודת החיתוך עם גבול התמונה  הימני
            secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageRightBorder.first, imageRightBorder.second);
            if (secondIntersection.has_value()) {
                secondVector.second = secondIntersection.value();
            }
        }

    }
    else if (deltaX < 0 && deltaY > 0) {
        // חישוב נקודות החיתוך עם גבול התמונה התחתון
        auto firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageLowerBorder.first, imageLowerBorder.second);
        auto secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageLowerBorder.first, imageLowerBorder.second);

        // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
        if (firstIntersection.has_value()) {
            firstVector.second = firstIntersection.value();
        }
        else {
            // חישוב נקודת החיתוך עם גבול התמונה  השמאלי
            firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageLeftBorder.first, imageLeftBorder.second);
            if (firstIntersection.has_value()) {
                firstVector.second = firstIntersection.value();
            }
        }
        if (secondIntersection.has_value()) {
            secondVector.second = secondIntersection.value();
        }
        else {
            // חישוב נקודת החיתוך עם גבול התמונה  השמאלי
            secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageLeftBorder.first, imageLeftBorder.second);
            if (secondIntersection.has_value()) {
                secondVector.second = secondIntersection.value();
            }
        }
    }
    else if (deltaX > 0 && deltaY < 0) {
        // חישוב נקודות החיתוך עם גבול התמונה העליון
        auto firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageUpperBorder.first, imageUpperBorder.second);
        auto secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageUpperBorder.first, imageUpperBorder.second);

        // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
        if (firstIntersection.has_value()) {
            firstVector.second = firstIntersection.value();
        }
        else {
            // חישוב נקודת החיתוך עם גבול התמונה  הימני
            firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageRightBorder.first, imageRightBorder.second);
            if (firstIntersection.has_value()) {
                firstVector.second = firstIntersection.value();
            }
        }
        if (secondIntersection.has_value()) {
            secondVector.second = secondIntersection.value();
        }
        else {
            // חישוב נקודת החיתוך עם גבול התמונה  הימני
            secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageRightBorder.first, imageRightBorder.second);
            if (secondIntersection.has_value()) {
                secondVector.second = secondIntersection.value();
            }
        }
    }
    else if (deltaX < 0 && deltaY < 0) {
        // חישוב נקודות החיתוך עם גבול התמונה העליון
        auto firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageUpperBorder.first, imageUpperBorder.second);
        auto secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageUpperBorder.first, imageUpperBorder.second);

        // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
        if (firstIntersection.has_value()) {
            firstVector.second = firstIntersection.value();
        }
        else {
            // חישוב נקודת החיתוך עם גבול התמונה  השמאלי
            firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageLeftBorder.first, imageLeftBorder.second);
            if (firstIntersection.has_value()) {
                firstVector.second = firstIntersection.value();
            }
        }
        if (secondIntersection.has_value()) {
            secondVector.second = secondIntersection.value();
        }
        else {
            // חישוב נקודת החיתוך עם גבול התמונה  השמאלי
            secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageLeftBorder.first, imageLeftBorder.second);
            if (secondIntersection.has_value()) {
                secondVector.second = secondIntersection.value();
            }
        }
    }
    else if (deltaX == 0 && deltaY < 0) {
        // חישוב נקודת החיתוך עם גבול התמונה העליון
        auto firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageUpperBorder.first, imageUpperBorder.second);
        auto secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageUpperBorder.first, imageUpperBorder.second);

        // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
        if (firstIntersection.has_value()) {
            firstVector.second = firstIntersection.value();
        }
        if (secondIntersection.has_value()) {
            secondVector.second = secondIntersection.value();
        }
    }
    else if (deltaX == 0 && deltaY > 0) {
        // חישוב נקודת החיתוך עם גבול התמונה התחתון
        auto firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageLowerBorder.first, imageLowerBorder.second);
        auto secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageLowerBorder.first, imageLowerBorder.second);

        // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
        if (firstIntersection.has_value()) {
            firstVector.second = firstIntersection.value();
        }
        if (secondIntersection.has_value()) {
            secondVector.second = secondIntersection.value();
        }
    }
    else if (deltaX > 0 && deltaY == 0) {

        // חישוב נקודת החיתוך עם גבול התמונה השמאלי
        auto firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageLeftBorder.first, imageLeftBorder.second);
        auto secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageLeftBorder.first, imageLeftBorder.second);

        // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
        if (firstIntersection.has_value()) {
            firstVector.second = firstIntersection.value();
        }
        if (secondIntersection.has_value()) {
            secondVector.second = secondIntersection.value();
        }
    }
    else if (deltaX < 0 && deltaY == 0) {
        // חישוב נקודת החיתוך עם גבול התמונה הימני
        auto firstIntersection = findIntersection(imageBorders, firstVector.first, firstVector.second, imageRightBorder.first, imageRightBorder.second);
        auto secondIntersection = findIntersection(imageBorders, secondVector.first, secondVector.second, imageRightBorder.first, imageRightBorder.second);

        // אם יש חיתוך עם גבולות התמונה, נשנה את נקודת הסיום של הווקטור
        if (firstIntersection.has_value()) {
            firstVector.second = firstIntersection.value();
        }
        if (secondIntersection.has_value()) {
            secondVector.second = secondIntersection.value();
        }
    }

    return std::make_pair(firstVector, secondVector);
}

//=================================================================================================
void drawMovementVectorsAndRect(
    const std::pair<int, int>& imageBorders,//??????????????????????????????????????????????????????????????
    cv::Mat& image,
    const cv::Rect& currRect,
    const std::pair<std::pair<cv::Point, cv::Point>, std::pair<cv::Point, cv::Point>>& movementVectors
){
    cv::line(image, movementVectors.first.first, movementVectors.first.second, cv::Scalar(0, 255, 0), 2); // ירוק
    cv::line(image, movementVectors.second.first, movementVectors.second.second, cv::Scalar(0, 255, 0), 2); // ירוק

    cv::rectangle(image, currRect, cv::Scalar(255, 0, 0), 2);//blue
}
//=================================================================================================
bool NewPrediction::isApproachingDangerZone(const Poligon& poligon, TrackedObject& trackedObject){
    auto movementVectors = getMovementDirection(trackedObject.bboxPrev, trackedObject.bboxCurr);
    if(poligon.doesLineIntersectPolygon(movementVectors.first) || poligon.doesLineIntersectPolygon(movementVectors.second))
		return true;
    return false;
}
//=================================================================================================
void NewPrediction::updateWarnings(std::unordered_map<int, TrackedObject>& predictedObjects)
{
    const std::pair<int, int>& imageBorders = IMAGE_BORDERS;
	for (auto& [id, trackedObject] : predictedObjects) {
        if (trackedObject.category == OBJECT_CATEGORY::TRAFFIC_LIGHT)
            continue;
        //CRITICAL_ZONE_1
        else if(zonesType[int(ZONE_TYPES::CRITICAL_ZONE_1)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)){
            trackedObject.zoneTypeIntersect = ZONE_TYPES::CRITICAL_ZONE_1;
            auto zoneTypeAndCatgoryPair = std::make_pair(ZONE_TYPES::CRITICAL_ZONE_1, trackedObject.category);
            if (zoneAndObjectToPriorityAndDescriptionMap.find(zoneTypeAndCatgoryPair) != zoneAndObjectToPriorityAndDescriptionMap.end()) {
                const auto& warningPriority_and_description = zoneAndObjectToPriorityAndDescriptionMap.at(zoneTypeAndCatgoryPair);
                trackedObject.warningPriority = warningPriority_and_description.first;
                trackedObject.warningDescription = warningPriority_and_description.second;
            }
			continue;
		}
        //CRITICAL_ZONE_2:
        //if person...critical./if car : positive speed: safe/negative speed: high
		else if (zonesType[int(ZONE_TYPES::CRITICAL_ZONE_2)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {
            trackedObject.zoneTypeIntersect = ZONE_TYPES::CRITICAL_ZONE_2;
            auto zoneTypeAndCatgoryPair = std::make_pair(ZONE_TYPES::CRITICAL_ZONE_2, trackedObject.category);
            if (zoneTypeAndCatgoryPair.second == OBJECT_CATEGORY::PERSON && zoneAndObjectToPriorityAndDescriptionMap.find(zoneTypeAndCatgoryPair) != zoneAndObjectToPriorityAndDescriptionMap.end()) {
                const auto& warningPriority_and_description = zoneAndObjectToPriorityAndDescriptionMap.at(zoneTypeAndCatgoryPair);
                trackedObject.warningPriority = warningPriority_and_description.first;
                trackedObject.warningDescription = warningPriority_and_description.second;
            }
            else {
                auto& distance = trackedObject.oldRelativSpeed;
                if (distance >= 0) {
					trackedObject.warningPriority = WarningPriority::Safe;
					trackedObject.warningDescription = "Car in CRITICAL_ZONE_2";
				}
				else {
					trackedObject.warningPriority = WarningPriority::High;
					trackedObject.warningDescription = "Veichle is slowing down in CRITICAL_ZONE_2";
				}
            }
            continue;
		}
        //TRACKED_ZONE
		else if (zonesType[int(ZONE_TYPES::TRACKED_ZONE)].first.doesRectIntersectPolygon(trackedObject.bboxCurr)) {
            //color it in the 	TRACKED_ZONE color
            trackedObject.zoneTypeIntersect = ZONE_TYPES::TRACKED_ZONE;
            if (isApproachingDangerZone(zonesType[int(ZONE_TYPES::CRITICAL_ZONE_1)].first, trackedObject)) {
                trackedObject.warningPriority = WarningPriority::High;
                trackedObject.warningDescription = "object is move to CRITICAL_ZONE_1";
                continue;
            }
			if (isApproachingDangerZone(zonesType[int(ZONE_TYPES::CRITICAL_ZONE_2)].first, trackedObject)) {
                if(trackedObject.category == OBJECT_CATEGORY::PERSON){
					trackedObject.warningPriority = WarningPriority::High;
					trackedObject.warningDescription = "person is move to CRITICAL_ZONE_2";
				}
				else{
					trackedObject.warningPriority = WarningPriority::Medium;
					trackedObject.warningDescription = "object is move to CRITICAL_ZONE_2";
				}
				continue;
			}
            else {//object is in TRACKED_ZONE but not in CRITICAL_ZONE_1 or CRITICAL_ZONE_2
			    trackedObject.warningPriority = WarningPriority::Low;
			    trackedObject.warningDescription = "object is in TRACKED_ZONE";
			    continue;
			}
        }
        else {//object is in CAR_HOOD or GENERAL
            trackedObject.zoneTypeIntersect = ZONE_TYPES::GENERAL;
			trackedObject.warningPriority = WarningPriority::Safe;
			trackedObject.warningDescription = "";
        }
	}
}
/* if(trackedObject.category != OBJECT_CATEGORY::CAR && trackedObject.category != OBJECT_CATEGORY::PERSON && trackedObject.category != OBJECT_CATEGORY::BICYCLE
     && trackedObject.category != OBJECT_CATEGORY::MOTORBIKE && trackedObject.category != OBJECT_CATEGORY::TRUCK
     && trackedObject.category != OBJECT_CATEGORY::BUS && trackedObject.category != OBJECT_CATEGORY::TRAIN)
     continue;*/
