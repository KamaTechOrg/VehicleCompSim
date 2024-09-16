#include "JsonLoader.h"

#include "constants.h"

JsonLoader::JsonLoader()
{
	_logicDataJsonFileName = constants::CONDITIONS_JSON_FILE_NAME;
	_guiDataJsonFileName =	 constants::GUI_DATA_JSON_FILE_NAME;
}

nlohmann::json JsonLoader::loadConditionsLogic()
{
	return nlohmann::json();
}

nlohmann::json JsonLoader::loadGuiData()
{
	return nlohmann::json();
}
