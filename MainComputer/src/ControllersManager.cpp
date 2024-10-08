#include "ControllersManager.h"

std::vector<std::string> ControllersManager::getControllersIDS() const
{
    std::vector<std::string> ids;
    for (const auto& controller : _controllers)
        ids.push_back(controller.first);
    return ids;
}

std::vector<std::string> ControllersManager::getControllersOptions(std::string controllerID) const
{
    if (_controllers.find(controllerID) != _controllers.end())
        return _controllers.at(controllerID);
    return std::vector<std::string>();
}