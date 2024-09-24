#include "data_manipulator.h"

int Data_manipulator::CRCalgo(char *data)
{
    uint16_t crc = 0xFFFF; 
    while (*data) {
        crc ^= (uint8_t)(*data++) << 8; 

        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021; 
            } else {
                crc <<= 1;
            }
        }
    }
    return crc & 0xFFFF; 
}

std::string Data_manipulator::int_to_str(int value)
{
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

        
            size_t pos3 = input.find('!', pos2 + 1);
            if (pos3 != std::string::npos)
            {
             
                std::string message = input.substr(pos2 + 1, pos3 - pos2 - 1);

               
                int EXcrc = std::stoi(input.substr(pos3 + 1));

               
                CanBus cb(source_id, dest_id, message, message.size());

                cb.crc = EXcrc;
                
           

                return cb;
            }
        }
    }

    return std::nullopt; 
}

std::pair<std::string,size_t> Data_manipulator::data_and_id_to_str(void *data, size_t size, int source_id, int dest_id)
{   
    char *charData = static_cast<char*>(data);
    int crc = CRCalgo(charData);
    std::ostringstream oss;
    oss << source_id << '!' << dest_id << '!';
    std::string all_data = oss.str();
    all_data.append((char *)(data), size);
    all_data += '!';
    std::string crcstr = int_to_str(crc);
    all_data += crcstr;


    return std::make_pair(all_data,all_data.size());

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
    oss << std::put_time(&now_tm, "%Y-%m-%d%H:%M:%S");
    
    return oss.str();
}

std::filesystem::path Data_manipulator::getTempFilePath(const std::string& fileName) {
    std::filesystem::path tempDir = std::filesystem::temp_directory_path();
    return tempDir / fileName; 
}


std::string Data_manipulator::readFileContents(const std::filesystem::path& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return "";
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n"; 
    }

    file.close();
    return content; 
}

std::string Data_manipulator::get_ip_server(const std::string &filename)
{
   std::filesystem::path p = getTempFilePath(filename);
   return readFileContents(p);
}

bool Data_manipulator::validateCRC(const std::string &input, int pos1, const char *buf, void *data)
{
       std::string input1 = input.substr(0, pos1);
    int EXcrc = std::stoi(input.substr(pos1 + 1, input.length() - pos1 - 1));
    char* message = const_cast<char*>(input1.c_str());

    if (Data_manipulator::CRCalgo(message) == EXcrc) {
        memcpy(data, buf, pos1);

        std::cout << "received = ";
        for (int i = 0; i < pos1; ++i) {
            std::cout << buf[i];
        }
        std::cout << std::endl;

        return true;
    } else {
        std::cout << "CRC check failed" << std::endl;
        memcpy(data, buf, input.length());
        return false;
    }
}

