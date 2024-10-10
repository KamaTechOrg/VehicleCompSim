#include "Prediction.h"
//=============================================================================
cv::Rect2d Prediction::predictNextPosition(const TrackedObject& trackedObject) const {
	const cv::Rect2d& currentPos = trackedObject.bboxCurr;
	const cv::Rect2d& previousPos = trackedObject.bboxPrev;

	// Calculate velocity (difference between current and previous positions).
	double dx = currentPos.x - previousPos.x;
	double dy = currentPos.y - previousPos.y;

	// Calculate the change in width and height.
	double dWidth = currentPos.width - previousPos.width;
	double dHeight = currentPos.height - previousPos.height;

	// Predict the next position and size by adding velocity and size change.
	return cv::Rect2d(
		(currentPos.x + dx) > 0 ? currentPos.x + dx : 0,                
		(currentPos.y + dy) > 0 ? currentPos.y + dy : 0,
		(currentPos.width + dWidth) > 0 ? currentPos.width + dWidth : 0,
		(currentPos.height + dHeight) > 0 ? currentPos.height + dHeight : 0
	);	
	//return cv::Rect2d(
	//	currentPos.x + dx,                  // Predict x position
	//	currentPos.y + dy,                  // Predict y position
	//	currentPos.width + dWidth,          // Predict width
	//	currentPos.height + dHeight         // Predict height
	//);
}
//=============================================================================
std::unordered_set<int> Prediction::analyzeSituation(double frameArea)
{
	//std::cout << "HELLO DAVID" << std::endl;
	//std::cout << m_predictedObjects.size() << std::endl;
	std::unordered_set<int> toColor;
	for (auto& [id, predictedObject] : m_predictedObjects)
	{
		if(WARNING_PERCENTAGE_MAP.find(predictedObject.category) == WARNING_PERCENTAGE_MAP.end())
		{
			std::cerr << "Error: category " << predictedObject.category << " is not in the warnings map" << std::endl;
			continue;
		}
		cv::Rect2d result = predictNextPosition(predictedObject);
		double predictionArea = result.area();
		double percentage = predictionArea / frameArea;
		//std::cout << "category: " << predictedObject.category << "percentage: " << percentage << std::endl;
		if (percentage > WARNING_PERCENTAGE_MAP.at(predictedObject.category))
		{
			std::cout << "WARNING!!! " << predictedObject.category
				<< " is too close. is taking " << percentage << "%" << std::endl;
			toColor.insert(id);
		}
	}
	return toColor;


}
//=============================================================================
void Prediction::setCanPredict(bool canPredict)
{
	m_canPredict = canPredict;
}
//=============================================================================
bool Prediction::getCanPredict() const
{
	return m_canPredict;
}
//=============================================================================
std::unordered_map<int, TrackedObject>& Prediction::getPredictedObjects()
{
	return m_predictedObjects;
}
//=============================================================================
bool Prediction::isApproachingDangerZone(const std::vector<cv::Point>& poligon, const TrackedObject& trackedObject)
{

	/*cv::Rect directionArea;
	return doesRectIntersectPolygon(poligon, nextPositionRect);*/

	return false;
}