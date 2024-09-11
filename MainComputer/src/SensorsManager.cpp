#include "SensorsManager.h"

std::vector<std::string> SensorsManager::getSensorsIDS() const
{
    std::vector<std::string> ids;
    for (const auto& sensor : _sensors)
        ids.push_back(sensor.first);
    return ids;
}

std::vector<int> SensorsManager::getPortNumbers() const
{
    std::vector<int> ports;
    for (const auto& sensor : _sensors)
        ports.push_back(sensor.second);
    return ports;
}
