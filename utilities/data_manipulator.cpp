#include "data_manipulator.h"



std::string Data_manipulator::int_to_str (int value){
        std::stringstream ss;
        ss << value;
        return ss.str();
    }


std::optional<CanBus> Data_manipulator::extract_id_and_data(const char *data, int len)
{
    std::string input(data, len);

    size_t pos1 = input.find('!');
    if (pos1 != std::string::npos)
    {
        int source_id = std::stoi(input.substr(0, pos1));
        size_t pos2 = input.find('!', pos1 + 1);

        if (pos2 != std::string::npos)
        {
            int dest_id = std::stoi(input.substr(pos1 + 1, pos2 - pos1 - 1));
            std::string message = input.substr(pos2 + 1);

            CanBus cb(source_id, dest_id, message, message.size());
          

            return cb; 
        }
    }

    return std::nullopt; 
}

std::string Data_manipulator::data_and_id_to_str(void *data, size_t size, int source_id, int dest_id)
{
    std::ostringstream oss;
    oss << source_id << '!' << dest_id << '!';
    std::string id_data = oss.str();
    id_data.append((char *)(data), size);

    return id_data;

}


std::string Data_manipulator:: getCurrentTime() {
    // Get current time point
    auto now = std::chrono::system_clock::now();
    
    // Convert to time_t to extract calendar time
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    
    // Convert to tm structure for local time
    std::tm now_tm = *std::localtime(&now_time_t);
    
    // Format time as a string
    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    
    return oss.str();
}
