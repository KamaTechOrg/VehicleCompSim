#pragma once

#include <unordered_map>
#include <string>
#include <vector>

class ControllersManager
{
public:
	std::vector<std::string> getControllersIDS() const;
	std::vector<int> getControllersPortNumbers() const;
	int getPortByID(const std::string& id) const;

private:
	// <"id", port number>
	inline static const std::unordered_map<std::string, int> _controllers = {
		{"Beep Controller", 8201},
		{"Dashboard Controller", 8202},
	};
};
