#include "data_manipulator.h"


std::string Data_manipulator::int_to_str (int value){
        std::stringstream ss;
        ss << value;
        return ss.str();
    }