#include <string>
#include <sstream>
#include <optional>

#include "canbus.h"


namespace Data_manipulator{
    std::string int_to_str (int);
    std::optional<CanBus> extract_id_and_data(const char *data, int len);
}
