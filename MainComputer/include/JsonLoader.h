#pragma once

#include "json.hpp"

class JsonLoader
{
public:
	JsonLoader();
	~JsonLoader() = default;

	nlohmann::json loadConditionsLogic() const;
	nlohmann::json loadGuiData() const;

private:
	std::string _logicDataJsonFileName;
	std::string _guiDataJsonFileName;
};