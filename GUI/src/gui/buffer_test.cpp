//
// Created by OWNER on 15/09/2024.
//

#include "buffer_test.h"
#include "state/globalstate.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

buffer_test::buffer_test(QObject *parent) : QObject(parent), m_logFile(QDir::currentPath() + "/A.log"), msg_counter(0) {
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &buffer_test::my_new_buffer);
    m_timer->start(1000);
}
buffer_test::~buffer_test() {

}

int buffer_test::random_src(){
    return 1 + std::rand() % 10; // Generates a number between 1 and 10
}

int buffer_test::random_dest(int src){
    int dest;
    do {
        dest = 1 + std::rand() % 10;
    } while (dest == src);
    return dest;
}

int buffer_test::random_int(){
    return 1 + std::rand() % 10000;
}
int buffer_test::random_double(){
    return 1.0 + (10000.0 - 1.0) * static_cast<double>(std::rand()) / RAND_MAX;
}
std::string buffer_test::random_msg(){
    int content_nsg = std::rand() % msg.size();
    std::string name = msg[content_nsg] + " goldberg";
    return name;
}
std::string buffer_test::timeStamp() {
    auto now = std::chrono::system_clock::now();
    auto itt = std::chrono::system_clock::to_time_t(now);

    // Adjust for your local time zone (e.g., UTC+3)
    std::tm* localTime = std::gmtime(&itt);
    localTime->tm_hour += 3; // Add your time zone offset

    std::ostringstream ss;
    ss << std::put_time(localTime, "%Y-%m-%dT%H:%M:%S");
    std::string timestamp = ss.str();
    return timestamp;
}

void buffer_test::sensor1() {
    std::string time = timeStamp();
    int src = 1;
    int dest = random_dest(src);
    int buffer_len = 200;
    char buffer[200];
    int index = 0;

    int new_int = random_int();
//    qInfo() << "original_int: " << new_int;
    std::memcpy(buffer + index, &new_int, sizeof(new_int));
    index += sizeof(new_int);

    double new_double = random_double();
//    qInfo() << "original_double: " << new_double;
    std::memcpy(buffer + index, &new_double, sizeof(new_double));
    index += sizeof(new_double);

    std::string new_msg = random_msg();
//    qInfo() << "original_msg: " << new_msg;
    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
    buffer[index + new_msg.length()] = '\0'; // Add the null terminator

//    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
    index += new_msg.length() + 1;  // Include null terminator
    buffer_len = index;

    // Create CSV string
    std::stringstream my_stream;
    my_stream << time << ","
              << src << ","
              << dest << ","
              << buffer_len << ","
              << std::string(buffer, buffer_len);  // Convert buffer to string

    // Print first 4 pieces
    std::vector<std::string> split_pieces;
    std::string piece;
    while (std::getline(my_stream, piece, ',')) {
        split_pieces.push_back(piece);
    }
//    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
//        std::cout << split_pieces[i] << std::endl;
//    }

    // Deserialize buffer
    if (split_pieces.size() > 4) {
        const std::string& buffer_str = split_pieces[4];
        const char* exit_buffer = buffer_str.c_str();
        int exit_index = 0;

        // Extract int (age)
        int exit_int;
        std::memcpy(&exit_int, exit_buffer + exit_index, sizeof(exit_int));
        exit_index += sizeof(exit_int);

        // Extract double (high)
        double exit_double;
        std::memcpy(&exit_double, exit_buffer + exit_index, sizeof(exit_double));
        exit_index += sizeof(exit_double);

        // Extract string (name)
        std::string exit_name1(exit_buffer + exit_index, 14);
        exit_index += 14;

//        std::cout << "Extracted age: " << exit_int << std::endl;
//        std::cout << "Extracted high: " << exit_double << std::endl;
//        std::cout << "Extracted name1: " << exit_name1 << std::endl;
    }

    QString converted_data = QString::fromStdString(my_stream.str());
    GlobalState::getInstance().new_test_buffer(converted_data);
}
void buffer_test::sensor2() {
    std::string time = timeStamp();
    int src = 2;
    int dest = random_dest(src);
    int buffer_len = 200;
    char buffer[200];
    int index = 0;

    double new_double = random_double();
//    qInfo() << "original_double: " << new_double;
    std::memcpy(buffer + index, &new_double, sizeof(new_double));
    index += sizeof(new_double);

    std::string new_msg = random_msg();
//    qInfo() << "original_msg: " << new_msg;
    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
    buffer[index + new_msg.length()] = '\0'; // Add the null terminator

//    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
    index += new_msg.length() + 1;  // Include null terminator
    buffer_len = index;

    int new_int = random_int();
//    qInfo() << "original_int: " << new_int;
    std::memcpy(buffer + index, &new_int, sizeof(new_int));
    index += sizeof(new_int);

    // Create CSV string
    std::stringstream my_stream;
    my_stream << time << ","
              << src << ","
              << dest << ","
              << buffer_len << ","
              << std::string(buffer, buffer_len);  // Convert buffer to string

    // Print first 4 pieces
    std::vector<std::string> split_pieces;
    std::string piece;
    while (std::getline(my_stream, piece, ',')) {
        split_pieces.push_back(piece);
    }
//    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
//        std::cout << split_pieces[i] << std::endl;
//    }

    // Deserialize buffer
    if (split_pieces.size() > 4) {
        const std::string& buffer_str = split_pieces[4];
        const char* exit_buffer = buffer_str.c_str();
        int exit_index = 0;

        // Extract double (high)
        double exit_double;
        std::memcpy(&exit_double, exit_buffer + exit_index, sizeof(exit_double));
        exit_index += sizeof(exit_double);

        // Extract string (name)
        std::string exit_name1(exit_buffer + exit_index, 14);
        exit_index += 15;

        // Extract int (age)
        int exit_int;
        std::memcpy(&exit_int, exit_buffer + exit_index, sizeof(exit_int));
        exit_index += sizeof(exit_int);

//        std::cout << "Extracted high: " << exit_double << std::endl;
//        std::cout << "Extracted name1: " << exit_name1 << std::endl;
//        std::cout << "Extracted age: " << exit_int << std::endl;
    }

    QString converted_data = QString::fromStdString(my_stream.str());
    GlobalState::getInstance().new_test_buffer(converted_data);
}

void buffer_test::sensor3() {
    std::string time = timeStamp();
    int src = 3;
    int dest = random_dest(src);
    int buffer_len = 200;
    char buffer[200];
    int index = 0;

    std::string new_msg = random_msg();
//    qInfo() << "original_msg1: " << new_msg;
    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
    buffer[index + new_msg.length()] = '\0'; // Add the null terminator

//    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
    index += new_msg.length() + 1;  // Include null terminator
    buffer_len = index;

    int new_int = random_int();
//    qInfo() << "original_int: " << new_int;
    std::memcpy(buffer + index, &new_int, sizeof(new_int));
    index += sizeof(new_int);

    double new_double = random_double();
//    qInfo() << "original_double: " << new_double;
    std::memcpy(buffer + index, &new_double, sizeof(new_double));
    index += sizeof(new_double);

    // Create CSV string
    std::stringstream my_stream;
    my_stream << time << ","
              << src << ","
              << dest << ","
              << buffer_len << ","
              << std::string(buffer, buffer_len);  // Convert buffer to string

    // Print first 4 pieces
    std::vector<std::string> split_pieces;
    std::string piece;
    while (std::getline(my_stream, piece, ',')) {
        split_pieces.push_back(piece);
    }
//    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
//        std::cout << split_pieces[i] << std::endl;
//    }

    // Deserialize buffer
    if (split_pieces.size() > 4) {
        const std::string& buffer_str = split_pieces[4];
        const char* exit_buffer = buffer_str.c_str();
        int exit_index = 0;

        // Extract string (name)
        std::string exit_name1(exit_buffer, 14);
        exit_index += 15;  // Include null terminator

        // Extract int (age)
        int exit_int;
        std::memcpy(&exit_int, exit_buffer + exit_index, sizeof(exit_int));
        exit_index += sizeof(exit_int);

        // Extract double (high)
        double exit_double;
        std::memcpy(&exit_double, exit_buffer + exit_index, sizeof(exit_double));
        exit_index += sizeof(exit_double);

//        std::cout << "Extracted name1: " << exit_name1 << std::endl;
//        std::cout << "Extracted age: " << exit_int << std::endl;
//        std::cout << "Extracted high: " << exit_double << std::endl;
    }

    QString converted_data = QString::fromStdString(my_stream.str());
    GlobalState::getInstance().new_test_buffer(converted_data);
}

void buffer_test::sensor4() {
    std::string time = timeStamp();
    int src = 4;
    int dest = random_dest(src);
    int buffer_len = 200;
    char buffer[200];
    int index = 0;

    std::string new_msg1 = random_msg();
//    qInfo() << "original_msg1: " << new_msg1;
    strncpy(buffer + index, new_msg1.c_str(), new_msg1.length());
    buffer[index + new_msg1.length()] = '\0'; // Add the null terminator

//    strncpy(buffer + index, new_msg1.c_str(), new_msg1.length());
    index +=  15;  // Include null terminator
    buffer_len = index;

    int new_int = random_int();
//    qInfo() << "original_int: " << new_int;
    std::memcpy(buffer + index, &new_int, sizeof(new_int));
    index += sizeof(new_int);

    double new_double = random_double();
//    qInfo() << "original_double: " << new_double;
    std::memcpy(buffer + index, &new_double, sizeof(new_double));
    index += sizeof(new_double);

    std::string new_msg2 = random_msg();
//    qInfo() << "original_msg2: " << new_msg2;
    strncpy(buffer + index, new_msg2.c_str(), new_msg2.length());
    buffer[index + new_msg2.length()] = '\0'; // Add the null terminator

//    strncpy(buffer + index, new_msg2.c_str(), new_msg2.length());
    index += 15;  // Include null terminator
    buffer_len = index;

    // Create CSV string
    std::stringstream my_stream;
    my_stream << time << ","
              << src << ","
              << dest << ","
              << buffer_len << ","
              << std::string(buffer, buffer_len);  // Convert buffer to string

    // Print first 4 pieces
    std::vector<std::string> split_pieces;
    std::string piece;
    while (std::getline(my_stream, piece, ',')) {
        split_pieces.push_back(piece);
    }
//    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
//        std::cout << split_pieces[i] << std::endl;
//    }

    // Deserialize buffer
    if (split_pieces.size() > 4) {
        const std::string& buffer_str = split_pieces[4];
        const char* exit_buffer = buffer_str.c_str();
        int exit_index = 0;

        // Extract string (name1)
        std::string exit_name1(exit_buffer, 14);
        exit_index += 15;  // Include null terminator

        // Extract int (age)
        int exit_int;
        std::memcpy(&exit_int, exit_buffer + exit_index, sizeof(exit_int));
        exit_index += sizeof(exit_int);

        // Extract double (high)
        double exit_double;
        std::memcpy(&exit_double, exit_buffer + exit_index, sizeof(exit_double));
        exit_index += sizeof(exit_double);

        // Extract string (name2)
        std::string exit_name2(exit_buffer + exit_index, exit_buffer + exit_index + 14);

//        std::cout << "Extracted name1: " << exit_name1 << std::endl;
//        std::cout << "Extracted age: " << exit_int << std::endl;
//        std::cout << "Extracted high: " << exit_double << std::endl;
//        std::cout << "Extracted name2: " << exit_name2 << std::endl;
    }

    QString converted_data = QString::fromStdString(my_stream.str());
    GlobalState::getInstance().new_test_buffer(converted_data);
}

void buffer_test::my_new_buffer() {
    int num = 1 + std::rand() % 4;
    if(num == 1){
        sensor1();
    }
    if(num == 2){
        sensor2();
    }
    if(num == 3){
        sensor4();
    }
    if(num == 4){
        sensor4();
    }
}


//void buffer_test::my_new_buffer() {
//    // Get current timestamp
//    auto now = std::chrono::system_clock::now();
//    auto itt = std::chrono::system_clock::to_time_t(now);
//    std::ostringstream ss;
//    ss << std::put_time(std::gmtime(&itt), "%Y-%m-%dT%H:%M:%S");
//    std::string timestamp = ss.str();
//
////    GlobalState::getInstance().currentProject()->models()
//
//    int src = 3;
//    int dest = 4;
//    int buffer_len = 200;
//
//    char buffer[200];
//    int index = 0;
//
//    // Serialize int
//    int age = 5678;
//    std::memcpy(buffer + index, &age, sizeof(age));
//    index += sizeof(age);
//
//    // Serialize double
//    double high = 4.555;
//    std::memcpy(buffer + index, &high, sizeof(high));
//    index += sizeof(high);
//
//    // Serialize string
//    std::string name = "yossi goldberg";
//    std::strcpy(buffer + index, name.c_str());
//    index += name.length() + 1;  // Include null terminator
//
//    // Update buffer_len to actual used size
//    buffer_len = index;
//
//    // Create CSV string
//    std::stringstream my_stream;
//    my_stream << timestamp << ","
//              << src << ","
//              << dest << ","
//              << buffer_len << ","
//              << std::string(buffer, buffer_len);  // Convert buffer to string
//
//    // Split CSV string
//    std::vector<std::string> split_pieces;
//    std::string piece;
//    while (std::getline(my_stream, piece, ',')) {
//        split_pieces.push_back(piece);
//    }
/////////////////////////////// for printing only
////
////    // Print first 4 pieces
////    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
////        std::cout << split_pieces[i] << std::endl;
////    }
////
////    // Deserialize buffer
////    if (split_pieces.size() > 4) {
////        const std::string& buffer_str = split_pieces[4];
////        const char* exit_buffer = buffer_str.c_str();
////        int exit_index = 0;
////
////        // Extract int (age)
////        int exit_age;
////        std::memcpy(&exit_age, exit_buffer + exit_index, sizeof(exit_age));
////        exit_index += sizeof(exit_age);
////
////        // Extract double (high)
////        double exit_high;
////        std::memcpy(&exit_high, exit_buffer + exit_index, sizeof(exit_high));
////        exit_index += sizeof(exit_high);
////
////        // Extract string (name)
////        std::string exit_name(exit_buffer + exit_index);
////
////        // Print extracted values
////        std::cout << "Extracted age: " << exit_age << std::endl;
////        std::cout << "Extracted high: " << exit_high << std::endl;
////        std::cout << "Extracted name: " << exit_name << std::endl;
////    }
/////////////////////////////// for printing only
//
//    QString converted_data = QString::fromStdString(my_stream.str());
//    GlobalState::getInstance().new_test_buffer(converted_data);
//}