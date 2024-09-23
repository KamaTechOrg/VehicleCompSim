#pragma once

#include "json.hpp"

class JsonLoader
{
public:
	JsonLoader();
	~JsonLoader() = default;

	nlohmann::json loadConditionsLogic() const;
	nlohmann::json::array_t loadGuiData() const;
	void saveConditionsLogic(const nlohmann::json &jsonData) const;
	void saveGuiData(const nlohmann::json &jsonData) const;

private:
	std::string _logicDataJsonFileName;
	std::string _guiDataJsonFileName;

	nlohmann::json generateDefaultLogicJson() const;
	nlohmann::json generateDefaultGuiDataJson() const;
};