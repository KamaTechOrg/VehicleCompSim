#include "Curved_road_detection.h"


float calculateSlope(const std::vector<int>& line) {
	if (line.size() != 4) {
		throw std::invalid_argument("The vector must contain 4 values: x1, y1, x2, y2.");
	}

	float x1 = line[0], y1 = line[1];
	float x2 = line[2], y2 = line[3];

	// Checking if the points are on the same vertical line (division by zero case)
	if (x2 - x1 == 0) {
		return 0;
		throw std::invalid_argument("The points create a vertical line, so the slope is undefined.");
	}

	// Calculating the slope
	return (y2 - y1) / (x2 - x1);
}

int Curved_road_detection(const std::vector<std::vector<int>>& lanes, float threshold) {
	if (lanes.size() != 2)
		return 0; // can't check for Curved.

	float left_slope = calculateSlope(lanes[0]);
	float right_slope = calculateSlope(lanes[1]);
	std::cout << "L: " << left_slope << " R: " << right_slope << " sum: " << left_slope + right_slope << std::endl;
	if (left_slope + right_slope > threshold)
	{
		return 2;
	}
	else if (left_slope + right_slope < 0 - threshold)
	{
		return 1;
	}
	return 0;
}