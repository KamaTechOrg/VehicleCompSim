#include <string>
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants {
	constexpr int MAX_ELAPSED_TIME = 10000;
	const std::string CONDITIONS_JSON_FILE_NAME = "conditions.json";
	const std::string GUI_DATA_JSON_FILE_NAME = "GUI_data.json";
	const std::string SERVER_IP = "172.232.208.10";
	const int SERVER_PORT = 8080;
}

#endif // CONSTANTS_H
