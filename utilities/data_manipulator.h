#pragma once

#include <string>
#include <sstream>
#include <optional>
#include <chrono>
#include <iomanip>
#include <ctime>


#include "canbus.h"

class CanBus ;
namespace Data_manipulator{
    std::string int_to_str (int);
    std::optional<CanBus> extract_id_and_data(const char *data, int len);
    std::string data_and_id_to_str (void *data, size_t size, int source_id, int dest_id);
    std::string getCurrentTime();
}
