#pragma once

#include <unordered_map>
#include <string>

class ControllersManager
{
public:
	std::vector<std::string> getControllersIDS() const;
	std::vector<std::string> getControllersOptions(std::string controllerID) const;

private:
	// <"id", options[]>
	inline static const std::unordered_map<std::string, std::vector<std::string>> _controllers = {
		{"Beep Controller", {
			"to close",
			"stop",
			"clear",
			}
		},
		{"Dashboard Controller", {
			"low fuel",
			"open backle",
			}
		},
	};
};