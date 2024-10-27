#pragma once
#include <vector>
#include <initializer_list>
#include <opencv2/opencv.hpp>
//#include "constants.h"

class Poligon
{
public:
	Poligon(std::vector<cv::Point>& points);
	Poligon(std::initializer_list<cv::Point> points);
	bool doesRectIntersectPolygon(const cv::Rect& rect) const;
	bool doesLineIntersectPolygon(const std::pair<cv::Point, cv::Point>& line) const;
	static bool doLinesIntersect(const cv::Point& p1, const cv::Point& p2, const cv::Point& q1, const cv::Point& q2);

	const std::vector<cv::Point>& getPoligon() const;
private:
	std::vector<cv::Point> m_poligon;
};

