#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class SensorsManager
{
public:
	std::vector<std::string> getSensorsIDS() const;
	std::vector<int> getPortNumbers() const;

private:

	// <"id", port number>
	inline static const std::unordered_map<std::string, int> _sensors = {
		{"Temperature Sensor", 8101},
		{"Reverse Distance Sensor", 8102},
		//{"Gear State", 8103},
		//{"Front Distance Sensor", 8104},
		//{"Buckle Sensor", 8105},
	};
};