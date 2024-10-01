#include "sendWarning.h"


void sendWarning(const std::string& msg)
{
    static const int id = 2345;

    //sentToCLient(m_id, msg);
    std::cout << "ID: " << id << " Warning: " << msg << std::endl;
}

