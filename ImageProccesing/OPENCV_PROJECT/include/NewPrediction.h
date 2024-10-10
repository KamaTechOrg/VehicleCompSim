#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include "tracking.h"
#include "util.h"
#include "constants.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "constants.h"
#include "Poligon.h"

class NewPrediction
{
public:
	static bool isApproachingDangerZone(const Poligon& tarpezoid, TrackedObject& trackedObject);
	static void updateWarnings(std::unordered_map<int, TrackedObject>& predictedObjects);

};

