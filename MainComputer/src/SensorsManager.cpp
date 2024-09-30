#include "SensorsManager.h"

std::vector<std::string> SensorsManager::getSensorsIDS() const
{
    std::vector<std::string> ids;
    for (const auto& sensor : _sensors)
        ids.push_back(sensor.first);
    return ids;
}

std::vector<std::string> SensorsManager::getSensorOptions(std::string sensorID) const
{
    if (_sensors.find(sensorID) != _sensors.end())
        return _sensors.at("sensorID");
    return std::vector<std::string>();
}