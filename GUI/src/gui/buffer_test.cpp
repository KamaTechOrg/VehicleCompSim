//
// Created by OWNER on 15/09/2024.
//

#include "buffer_test.h"
#include "state/globalstate.h"
#include "constants.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

buffer_test::buffer_test(QObject *parent) {
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &buffer_test::testExtractBufferData);
    m_timer->start(1000);
}
buffer_test::~buffer_test() {

}
void buffer_test::testExtractBufferData() {
    // Create a sample buffer with all supported types
    QByteArray testBuffer;
    QDataStream stream(&testBuffer, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    int src_id = 1;
    int dest_id = 2;
    int len = 50;

// Compose the melody:
    std::ostringstream oss;
    oss << time << ',' << src_id << ',' << dest_id << ',' << len << ',';
    std::string data = oss.str();
    char buffer[1024];
    buffer[0] = 'b';
    buffer[1] = ',';
    qInfo() << buffer[0];
//
//    QString tt = "yossi";
//    QByteArray ttBytes = tt.toUtf8();
//    int startPos = 2;
//    memcpy(buffer + startPos, ttBytes.constData(), ttBytes.size());
//    // Optionally, null-terminate the string in the buffer
//    buffer[startPos + ttBytes.size()] = '\0';
//    startPos + ttBytes.size();
//    buffer[startPos] = ',';
//    startPos + 1;
//    int time = 222;
//    qint32 littleEndianTime = qToLittleEndian(static_cast<qint32>(time));
//    memcpy(buffer + startPos, &littleEndianTime, sizeof(qint32));
//
    GlobalState::getInstance().new_test_buffer(buffer, 1024);



//    int time = 222;
//    int src_id = 1;
//    int dest_id = 2;
//    int len = 50;
//    bufferr << time << ',' << src_id << ',' << dest_id << ',' << len << ',';
//
//    // int32_t
//    qint32 testInt32 = -12345;
//    bufferr << testInt32;

//    // uint32_t
//    quint32 testUint32 = 54321;
//    bufferr << testUint32;
//
//    // char*
//    const char *testString = "Hello, World!";
//    bufferr.writeRawData(testString, 14); // Including null terminator
//
//    // float
//    float testFloat = 3.14159f;
//    bufferr.writeRawData(reinterpret_cast<const char *>(&testFloat), sizeof(float));
//
//    // double
//    double testDouble = 2.71828182845904;
//    bufferr.writeRawData(reinterpret_cast<const char *>(&testDouble), sizeof(double));
//
//    // bool (3 booleans)
//    quint8 testBools = 0b00000101; // true, false, true
//    bufferr << testBools;
//    QFile outputFile(R"(C:\mobileye_project\VehicleCompSim\GUI\src\gui\buffer)");
//    if (outputFile.open(QIODevice::WriteOnly)) {
//        QDataStream outStream(&outputFile);
//        outStream.setByteOrder(QDataStream::LittleEndian);
//        outStream << testBuffer;
//        outputFile.close();
//        // Successfully wrote serialized data to the file!
//    } else {
//        // Handle the error (e.g., file couldn't be opened)
//    }
//    QByteArray buffer;
//    QFile inputFile(R"(C:\mobileye_project\VehicleCompSim\GUI\src\gui\buffer)");
//    if (inputFile.open(QIODevice::ReadOnly)) {
//        QDataStream inStream(&inputFile);
//        inStream.setByteOrder(QDataStream::LittleEndian);
//        inStream >> buffer;
//        inputFile.close();
//        // Successfully read data into buffer!
//    } else {
//        // Handle the error (e.g., file couldn't be opened)
//    }
//
//
//
//
//    // Convert the buffer to base64
//    QString base64String = testBuffer.toBase64();
//    GlobalState::getInstance().new_test_buffer(base64String);
}
//    QByteArray originalData; // Your original QByteArray
//    QString base64String = QString::fromLatin1(originalData.toBase64());
//    GlobalState::getInstance().new_test_buffer(base64String);
//}
//
//int buffer_test::random_src(){
//    return 1 + std::rand() % 10; // Generates a number between 1 and 10
//}
//
//int buffer_test::random_dest(int src){
//    int dest;
//    do {
//        dest = 1 + std::rand() % 10;
//    } while (dest == src);
//    return dest;
//}
//
//int buffer_test::random_int(){
//    return 1 + std::rand() % 10000;
//}
//int buffer_test::random_double(){
//    return 1.0 + (10000.0 - 1.0) * static_cast<double>(std::rand()) / RAND_MAX;
//}
//std::string buffer_test::random_msg(){
//    int content_nsg = std::rand() % msg.size();
//    std::string name = msg[content_nsg] + " goldberg";
//    return name;
//}
//std::string buffer_test::timeStamp() {
//    auto now = std::chrono::system_clock::now();
//    auto itt = std::chrono::system_clock::to_time_t(now);
//
//    // Adjust for your local time zone (e.g., UTC+3)
//    std::tm* localTime = std::gmtime(&itt);
//    localTime->tm_hour += 3; // Add your time zone offset
//
//    std::ostringstream ss;
//    ss << std::put_time(localTime, "%Y-%m-%dT%H:%M:%S");
//    std::string timestamp = ss.str();
//    return timestamp;
//}
//
//void buffer_test::sensor1() {
//    std::string time = timeStamp();
//    int src = 1;
//    int dest = random_dest(src);
//    int buffer_len = 200;
//    char buffer[200];
//    int index = 0;
//
//    int new_int = random_int();
////    qInfo() << "original_int: " << new_int;
//    std::memcpy(buffer + index, &new_int, sizeof(new_int));
//    index += sizeof(new_int);
//
//    double new_double = random_double();
////    qInfo() << "original_double: " << new_double;
//    std::memcpy(buffer + index, &new_double, sizeof(new_double));
//    index += sizeof(new_double);
//
//    std::string new_msg = random_msg();
////    qInfo() << "original_msg: " << new_msg;
//    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
//    buffer[index + new_msg.length()] = '\0'; // Add the null terminator
//
////    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
//    index += new_msg.length() + 1;  // Include null terminator
//    buffer_len = index;
//
//    // Create CSV string
//    std::stringstream my_stream;
//    my_stream << time << ","
//              << src << ","
//              << dest << ","
//              << buffer_len << ","
//              << std::string(buffer, buffer_len);  // Convert buffer to string
//
//    // Print first 4 pieces
//    std::vector<std::string> split_pieces;
//    std::string piece;
//    while (std::getline(my_stream, piece, ',')) {
//        split_pieces.push_back(piece);
//    }
////    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
////        std::cout << split_pieces[i] << std::endl;
////    }
//
//    // Deserialize buffer
//    if (split_pieces.size() > 4) {
//        const std::string& buffer_str = split_pieces[4];
//        const char* exit_buffer = buffer_str.c_str();
//        int exit_index = 0;
//
//        // Extract int (age)
//        int exit_int;
//        std::memcpy(&exit_int, exit_buffer + exit_index, sizeof(exit_int));
//        exit_index += sizeof(exit_int);
//
//        // Extract double (high)
//        double exit_double;
//        std::memcpy(&exit_double, exit_buffer + exit_index, sizeof(exit_double));
//        exit_index += sizeof(exit_double);
//
//        // Extract string (name)
//        std::string exit_name1(exit_buffer + exit_index, 14);
//        exit_index += 14;
//
////        std::cout << "Extracted age: " << exit_int << std::endl;
////        std::cout << "Extracted high: " << exit_double << std::endl;
////        std::cout << "Extracted name1: " << exit_name1 << std::endl;
//    }
//
//    QString converted_data = QString::fromStdString(my_stream.str());
//    GlobalState::getInstance().new_test_buffer(converted_data);
//}
//void buffer_test::sensor2() {
//    std::string time = timeStamp();
//    int src = 2;
//    int dest = random_dest(src);
//    int buffer_len = 200;
//    char buffer[200];
//    int index = 0;
//
//    double new_double = random_double();
////    qInfo() << "original_double: " << new_double;
//    std::memcpy(buffer + index, &new_double, sizeof(new_double));
//    index += sizeof(new_double);
//
//    std::string new_msg = random_msg();
////    qInfo() << "original_msg: " << new_msg;
//    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
//    buffer[index + new_msg.length()] = '\0'; // Add the null terminator
//
////    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
//    index += new_msg.length() + 1;  // Include null terminator
//    buffer_len = index;
//
//    int new_int = random_int();
////    qInfo() << "original_int: " << new_int;
//    std::memcpy(buffer + index, &new_int, sizeof(new_int));
//    index += sizeof(new_int);
//
//    // Create CSV string
//    std::stringstream my_stream;
//    my_stream << time << ","
//              << src << ","
//              << dest << ","
//              << buffer_len << ","
//              << std::string(buffer, buffer_len);  // Convert buffer to string
//
//    // Print first 4 pieces
//    std::vector<std::string> split_pieces;
//    std::string piece;
//    while (std::getline(my_stream, piece, ',')) {
//        split_pieces.push_back(piece);
//    }
////    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
////        std::cout << split_pieces[i] << std::endl;
////    }
//
//    // Deserialize buffer
//    if (split_pieces.size() > 4) {
//        const std::string& buffer_str = split_pieces[4];
//        const char* exit_buffer = buffer_str.c_str();
//        int exit_index = 0;
//
//        // Extract double (high)
//        double exit_double;
//        std::memcpy(&exit_double, exit_buffer + exit_index, sizeof(exit_double));
//        exit_index += sizeof(exit_double);
//
//        // Extract string (name)
//        std::string exit_name1(exit_buffer + exit_index, 14);
//        exit_index += 15;
//
//        // Extract int (age)
//        int exit_int;
//        std::memcpy(&exit_int, exit_buffer + exit_index, sizeof(exit_int));
//        exit_index += sizeof(exit_int);
//
////        std::cout << "Extracted high: " << exit_double << std::endl;
////        std::cout << "Extracted name1: " << exit_name1 << std::endl;
////        std::cout << "Extracted age: " << exit_int << std::endl;
//    }
//
//    QString converted_data = QString::fromStdString(my_stream.str());
//    GlobalState::getInstance().new_test_buffer(converted_data);
//}
//
//void buffer_test::sensor3() {
//    std::string time = timeStamp();
//    int src = 3;
//    int dest = random_dest(src);
//    int buffer_len = 200;
//    char buffer[200];
//    int index = 0;
//
//    std::string new_msg = random_msg();
////    qInfo() << "original_msg1: " << new_msg;
//    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
//    buffer[index + new_msg.length()] = '\0'; // Add the null terminator
//
////    strncpy(buffer + index, new_msg.c_str(), new_msg.length());
//    index += new_msg.length() + 1;  // Include null terminator
//    buffer_len = index;
//
//    int new_int = random_int();
////    qInfo() << "original_int: " << new_int;
//    std::memcpy(buffer + index, &new_int, sizeof(new_int));
//    index += sizeof(new_int);
//
//    double new_double = random_double();
////    qInfo() << "original_double: " << new_double;
//    std::memcpy(buffer + index, &new_double, sizeof(new_double));
//    index += sizeof(new_double);
//
//    // Create CSV string
//    std::stringstream my_stream;
//    my_stream << time << ","
//              << src << ","
//              << dest << ","
//              << buffer_len << ","
//              << std::string(buffer, buffer_len);  // Convert buffer to string
//
//    // Print first 4 pieces
//    std::vector<std::string> split_pieces;
//    std::string piece;
//    while (std::getline(my_stream, piece, ',')) {
//        split_pieces.push_back(piece);
//    }
////    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
////        std::cout << split_pieces[i] << std::endl;
////    }
//
//    // Deserialize buffer
//    if (split_pieces.size() > 4) {
//        const std::string& buffer_str = split_pieces[4];
//        const char* exit_buffer = buffer_str.c_str();
//        int exit_index = 0;
//
//        // Extract string (name)
//        std::string exit_name1(exit_buffer, 14);
//        exit_index += 15;  // Include null terminator
//
//        // Extract int (age)
//        int exit_int;
//        std::memcpy(&exit_int, exit_buffer + exit_index, sizeof(exit_int));
//        exit_index += sizeof(exit_int);
//
//        // Extract double (high)
//        double exit_double;
//        std::memcpy(&exit_double, exit_buffer + exit_index, sizeof(exit_double));
//        exit_index += sizeof(exit_double);
//
////        std::cout << "Extracted name1: " << exit_name1 << std::endl;
////        std::cout << "Extracted age: " << exit_int << std::endl;
////        std::cout << "Extracted high: " << exit_double << std::endl;
//    }
//
//    QString converted_data = QString::fromStdString(my_stream.str());
//    GlobalState::getInstance().new_test_buffer(converted_data);
//}
//
//void buffer_test::sensor4() {
//    std::string time = timeStamp();
//    int src = 4;
//    int dest = random_dest(src);
//    int buffer_len = 200;
//    char buffer[200];
//    int index = 0;
//
//    std::string new_msg1 = random_msg();
////    qInfo() << "original_msg1: " << new_msg1;
//    strncpy(buffer + index, new_msg1.c_str(), new_msg1.length());
//    buffer[index + new_msg1.length()] = '\0'; // Add the null terminator
//
////    strncpy(buffer + index, new_msg1.c_str(), new_msg1.length());
//    index +=  15;  // Include null terminator
//    buffer_len = index;
//
//    int new_int = random_int();
////    qInfo() << "original_int: " << new_int;
//    std::memcpy(buffer + index, &new_int, sizeof(new_int));
//    index += sizeof(new_int);
//
//    double new_double = random_double();
////    qInfo() << "original_double: " << new_double;
//    std::memcpy(buffer + index, &new_double, sizeof(new_double));
//    index += sizeof(new_double);
//
//    std::string new_msg2 = random_msg();
////    qInfo() << "original_msg2: " << new_msg2;
//    strncpy(buffer + index, new_msg2.c_str(), new_msg2.length());
//    buffer[index + new_msg2.length()] = '\0'; // Add the null terminator
//
////    strncpy(buffer + index, new_msg2.c_str(), new_msg2.length());
//    index += 15;  // Include null terminator
//    buffer_len = index;
//
//    // Create CSV string
//    std::stringstream my_stream;
//    my_stream << time << ","
//              << src << ","
//              << dest << ","
//              << buffer_len << ","
//              << std::string(buffer, buffer_len);  // Convert buffer to string
//
//    // Print first 4 pieces
//    std::vector<std::string> split_pieces;
//    std::string piece;
//    while (std::getline(my_stream, piece, ',')) {
//        split_pieces.push_back(piece);
//    }
////    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
////        std::cout << split_pieces[i] << std::endl;
////    }
//
//    // Deserialize buffer
//    if (split_pieces.size() > 4) {
//        const std::string& buffer_str = split_pieces[4];
//        const char* exit_buffer = buffer_str.c_str();
//        int exit_index = 0;
//
//        // Extract string (name1)
//        std::string exit_name1(exit_buffer, 14);
//        exit_index += 15;  // Include null terminator
//
//        // Extract int (age)
//        int exit_int;
//        std::memcpy(&exit_int, exit_buffer + exit_index, sizeof(exit_int));
//        exit_index += sizeof(exit_int);
//
//        // Extract double (high)
//        double exit_double;
//        std::memcpy(&exit_double, exit_buffer + exit_index, sizeof(exit_double));
//        exit_index += sizeof(exit_double);
//
//        // Extract string (name2)
//        std::string exit_name2(exit_buffer + exit_index, exit_buffer + exit_index + 14);
//
////        std::cout << "Extracted name1: " << exit_name1 << std::endl;
////        std::cout << "Extracted age: " << exit_int << std::endl;
////        std::cout << "Extracted high: " << exit_double << std::endl;
////        std::cout << "Extracted name2: " << exit_name2 << std::endl;
//    }
//
//    QString converted_data = QString::fromStdString(my_stream.str());
//    GlobalState::getInstance().new_test_buffer(converted_data);
//}
//
//void buffer_test::my_new_buffer() {
//    int num = 1 + std::rand() % 4;
//    if(num == 1){
//        sensor1();
//    }
//    if(num == 2){
//        sensor2();
//    }
//    if(num == 3){
//        sensor4();
//    }
//    if(num == 4){
//        sensor4();
//    }
//}
//
//
////void buffer_test::my_new_buffer() {
////    // Get current timestamp
////    auto now = std::chrono::system_clock::now();
////    auto itt = std::chrono::system_clock::to_time_t(now);
////    std::ostringstream ss;
////    ss << std::put_time(std::gmtime(&itt), "%Y-%m-%dT%H:%M:%S");
////    std::string timestamp = ss.str();
////
//////    GlobalState::getInstance().currentProject()->models()
////
////    int src = 3;
////    int dest = 4;
////    int buffer_len = 200;
////
////    char buffer[200];
////    int index = 0;
////
////    // Serialize int
////    int age = 5678;
////    std::memcpy(buffer + index, &age, sizeof(age));
////    index += sizeof(age);
////
////    // Serialize double
////    double high = 4.555;
////    std::memcpy(buffer + index, &high, sizeof(high));
////    index += sizeof(high);
////
////    // Serialize string
////    std::string name = "yossi goldberg";
////    std::strcpy(buffer + index, name.c_str());
////    index += name.length() + 1;  // Include null terminator
////
////    // Update buffer_len to actual used size
////    buffer_len = index;
////
////    // Create CSV string
////    std::stringstream my_stream;
////    my_stream << timestamp << ","
////              << src << ","
////              << dest << ","
////              << buffer_len << ","
////              << std::string(buffer, buffer_len);  // Convert buffer to string
////
////    // Split CSV string
////    std::vector<std::string> split_pieces;
////    std::string piece;
////    while (std::getline(my_stream, piece, ',')) {
////        split_pieces.push_back(piece);
////    }
///////////////////////////////// for printing only
//////
//////    // Print first 4 pieces
//////    for (int i = 0; i < 4 && i < split_pieces.size(); i++) {
//////        std::cout << split_pieces[i] << std::endl;
//////    }
//////
//////    // Deserialize buffer
//////    if (split_pieces.size() > 4) {
//////        const std::string& buffer_str = split_pieces[4];
//////        const char* exit_buffer = buffer_str.c_str();
//////        int exit_index = 0;
//////
//////        // Extract int (age)
//////        int exit_age;
//////        std::memcpy(&exit_age, exit_buffer + exit_index, sizeof(exit_age));
//////        exit_index += sizeof(exit_age);
//////
//////        // Extract double (high)
//////        double exit_high;
//////        std::memcpy(&exit_high, exit_buffer + exit_index, sizeof(exit_high));
//////        exit_index += sizeof(exit_high);
//////
//////        // Extract string (name)
//////        std::string exit_name(exit_buffer + exit_index);
//////
//////        // Print extracted values
//////        std::cout << "Extracted age: " << exit_age << std::endl;
//////        std::cout << "Extracted high: " << exit_high << std::endl;
//////        std::cout << "Extracted name: " << exit_name << std::endl;
//////    }
///////////////////////////////// for printing only
////
////    QString converted_data = QString::fromStdString(my_stream.str());
////    GlobalState::getInstance().new_test_buffer(converted_data);
////}