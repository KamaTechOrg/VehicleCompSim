#include "JsonLoader.h"

#include <QDebug>
#include <fstream>
#include "constants.h"

JsonLoader::JsonLoader()
{
	_logicDataJsonFileName = constants::CONDITIONS_JSON_FILE_NAME;
	_guiDataJsonFileName =	 constants::GUI_DATA_JSON_FILE_NAME;
}

nlohmann::json JsonLoader::loadConditionsLogic() const
{
    std::ifstream file(_logicDataJsonFileName);
    if (!file.is_open())
    {
        qWarning() <<  "Could not open JSON file: " << _logicDataJsonFileName.c_str();
        return generateDefaultLogicJson();
    }

    nlohmann::json jsonData;
    file >> jsonData;
    file.close();

    return jsonData;
}

nlohmann::json JsonLoader::loadGuiData() const
{
    std::ifstream file(_guiDataJsonFileName);
    if (!file.is_open()) {
        qDebug() << "Failed to open file: " << QString::fromStdString(_guiDataJsonFileName);
        return {};
    }

    nlohmann::json jsonData;
    try {
        file >> jsonData;
    }
    catch (const nlohmann::json::parse_error& e) {
        qDebug() << "JSON parse error: " << e.what();
        file.close();
        return generateDefaultGuiDataJson();
    }
    file.close();

    qDebug() << "Loaded JSON data: " << QString::fromStdString(jsonData.dump(4));

    // Check if the keys exist in the JSON data
    if (jsonData.contains("conditions") && jsonData.contains("actions")) {
        return jsonData;
    }
    else {
        qDebug() << "JSON data missing expected keys: 'conditions' or 'actions'.";
        return generateDefaultGuiDataJson();
    }

    return generateDefaultGuiDataJson();
}


void JsonLoader::saveConditionsLogic(const nlohmann::json& jsonData) const
{
    std::ofstream file(_logicDataJsonFileName);
    if (!file.is_open()) {
        qWarning() << "Could not open file for writing: " << QString::fromStdString(_logicDataJsonFileName);
        return;
    }

    try {
        file << jsonData.dump(4);  
    }
    catch (const std::exception& e) {
        qWarning() << "Error writing JSON to file: " << e.what();
    }

    file.close();
}

void JsonLoader::saveGuiData(const nlohmann::json& jsonData) const
{
    std::ofstream file(_guiDataJsonFileName);
    if (!file.is_open()) {
        qWarning() << "Could not open file for writing: " << QString::fromStdString(_guiDataJsonFileName);
        return;
    }

    try {
        file << jsonData.dump(4);  
    }
    catch (const std::exception& e) {
        qWarning() << "Error writing JSON to file: " << e.what();
    }

    file.close();
}

nlohmann::json JsonLoader::generateDefaultLogicJson() const
{
    return {
        {"conditions", {}},
        {"actions", nlohmann::json::array()}
    };
}

nlohmann::json JsonLoader::generateDefaultGuiDataJson() const
{
    return {
        {"conditions", nlohmann::json::array()},
        {"actions", nlohmann::json::array()}
    };
}
