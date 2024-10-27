#include "InputData.h"
#include "constants.h"
//======================================================================================
InputData& InputData::getInstance() {
    static InputData instance;
    return instance;
}
//======================================================================================
std::vector<std::pair<Poligon, ZONE_TYPES>>& InputData::getZonesPoligon(const std::string& videoName) {
    return m_videoZonesPoligons.at(videoName);
}
//======================================================================================
const std::unordered_map<std::string, std::vector<std::pair<Poligon, ZONE_TYPES>>>& InputData::getAllVideosPoligons() const
{
    return m_videoZonesPoligons;
}
//======================================================================================
const std::string& InputData::getLaneDetectionVideo() const
{
    return m_laneDetectionVideo;
}
//======================================================================================
const std::string& InputData::getVideosDirPath() const
{
    return m_videosDirPath;
}
//======================================================================================
InputData::InputData() {
	
    std::ifstream file(VIDEO_DATA);
    nlohmann::json jsonData;
    file >> jsonData;

    m_laneDetectionVideo = jsonData["lane_detection_video"];
    m_videosDirPath = VIDEO_DIR;
    loadVideoData(jsonData["tracking"]);
}
//======================================================================================
void InputData::loadVideoData(const nlohmann::json& jsonData) {
    int currVideoIdx = 0;
    for (const auto& video : jsonData["videos"]) {
        std::string videoName = video["name"];
        std::vector<std::pair<Poligon, ZONE_TYPES>> zonesPoligon;
        
        zonesPoligon.push_back({ createPoligonFromJSON(video["zones"]["CRITICAL_ZONE_1"]), ZONE_TYPES::CRITICAL_ZONE_1 });
        zonesPoligon.push_back({ createPoligonFromJSON(video["zones"]["CRITICAL_ZONE_2"]), ZONE_TYPES::CRITICAL_ZONE_2 });
        zonesPoligon.push_back({ createTrackedZonePoligon(jsonData, currVideoIdx), ZONE_TYPES::TRACKED_ZONE });
        zonesPoligon.push_back({ createPoligonFromJSON(video["zones"]["CAR_HOOD"]), ZONE_TYPES::CAR_HOOD });

        m_videoZonesPoligons[videoName] = zonesPoligon;
        ++currVideoIdx;
    }
}
//======================================================================================
Poligon InputData::createPoligonFromJSON(const nlohmann::json& zoneData) {
    cv::Point DL(zoneData["DL"][0], zoneData["DL"][1]);
    cv::Point DR(zoneData["DR"][0], zoneData["DR"][1]);
    cv::Point UR(zoneData["UR"][0], zoneData["UR"][1]);
    cv::Point UL(zoneData["UL"][0], zoneData["UL"][1]);

    std::vector<cv::Point> points = { DL, DR, UR, UL };
    return Poligon(points);
}
//======================================================================================
Poligon InputData::createTrackedZonePoligon(const nlohmann::json& jsonData, int currVideoIdx) {
    const nlohmann::json& zonesData = jsonData["videos"][currVideoIdx]["zones"];
    const int base = 200;
    const float  percent = (float(zonesData["CRITICAL_ZONE_2"]["UR"][0]) - float(zonesData["CRITICAL_ZONE_2"]["UL"][0])) / (float(zonesData["CRITICAL_ZONE_1"]["DR"][0]) - float(zonesData["CRITICAL_ZONE_1"]["DL"][0]));

    cv::Point DL(zonesData["CRITICAL_ZONE_1"]["DL"][0] - base, zonesData["CRITICAL_ZONE_1"]["DL"][1]);
    cv::Point DR(zonesData["CRITICAL_ZONE_1"]["DR"][0] + base, zonesData["CRITICAL_ZONE_1"]["DR"][1]);
    cv::Point UR(zonesData["CRITICAL_ZONE_2"]["UR"][0] + int(percent * float(base)), zonesData["CRITICAL_ZONE_2"]["UL"][1]);
    cv::Point UL(zonesData["CRITICAL_ZONE_2"]["UL"][0] - int(percent * float(base)), zonesData["CRITICAL_ZONE_2"]["UL"][1]);

    std::vector<cv::Point> points = { DL, DR, UR, UL };
    return Poligon(points);
}
//======================================================================================