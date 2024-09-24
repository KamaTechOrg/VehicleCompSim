#pragma once

#include <filesystem>
#include <optional>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <ctime>

#include "canbus.h"

class CanBus;
namespace Data_manipulator
{
    int CRCalgo(char *data);
    std::string int_to_str(int);
    std::optional<CanBus> extract_id_and_data(const char *data, int len);
    std::pair<std::string, size_t> data_and_id_to_str(void *data, size_t size, int source_id, int dest_id);
    std::string getCurrentTime();
    std::filesystem::path getTempFilePath(const std::string &fileName);
    std::string readFileContents(const std::filesystem::path &filePath);
    std::string get_ip_server(const std::string & filename);

}
