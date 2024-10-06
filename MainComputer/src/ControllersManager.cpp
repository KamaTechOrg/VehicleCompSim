#include "ControllersManager.h"
#include <vector>

std::vector<std::string> ControllersManager::getControllersIDS() const
{
    std::vector<std::string> ids;
    for (const auto& controller : _controllers)
        ids.push_back(controller.first);
    return ids;
}

std::vector<int> ControllersManager::getControllersPortNumbers() const
{
    std::vector<int> ports;
    for (const auto& controller : _controllers)
        ports.push_back(controller.second);
    return ports;
}

int ControllersManager::getPortByID(const std::string& id) const
{
    auto it = _controllers.find(id);
    if (it != _controllers.end()) {
        return it->second; 
    }
    return -1; 
}
