#pragma once

#include "json.hpp"

class JsonLoader
{
public:
	JsonLoader();
	~JsonLoader() = default;

	nlohmann::json loadConditionsLogic();
	nlohmann::json loadGuiData();

private:
	std::string _logicDataJsonFileName;
	std::string _guiDataJsonFileName;
};