#pragma once

#include "Poligon.h"
#include "util.h"
#include "json.hpp"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

class InputData {
public:
    static InputData& getInstance();
    std::vector<std::pair<Poligon, ZONE_TYPES>>& getZonesPoligon(const std::string& videoName);
    const std::unordered_map<std::string, std::vector<std::pair<Poligon, ZONE_TYPES>>>& getAllVideosPoligons() const;

    const std::string& getLaneDetectionVideo() const;
    const std::string& getVideosDirPath() const;

    InputData(const InputData&) = delete;
    InputData& operator=(const InputData&) = delete;
private:
    std::unordered_map<std::string, std::vector<std::pair<Poligon, ZONE_TYPES>>> m_videoZonesPoligons;
    std::string m_laneDetectionVideo;
    std::string m_videosDirPath;

    InputData();
    void loadVideoData(const nlohmann::json& jsonData);
    Poligon createPoligonFromJSON(const nlohmann::json& zoneData);
    Poligon createTrackedZonePoligon(const nlohmann::json& jsonData, int currVideoIdx);
};
