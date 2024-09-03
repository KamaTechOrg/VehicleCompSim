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