#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class SensorsManager
{
public:
	std::vector<std::string> getSensorsIDS() const;
	std::vector<std::string> getSensorOptions(std::string sensorID) const;

private:

	// <"id", port number>
	inline static const std::unordered_map<std::string, std::vector<std::string>> _sensors = {
		{"Temperature Sensor", {}},
		{"Reverse Distance Sensor", {}},
		//{"Gear State", 8103},
		//{"Front Distance Sensor", 8104},
		//{"Buckle Sensor", 8105},
	};
};