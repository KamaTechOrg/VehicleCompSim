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
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>


#include <string.h>
#include <stdint.h>


buffer_test::buffer_test(QObject *parent) {
    m_buffer_test_timer = new QTimer(this);
    connect(m_buffer_test_timer, &QTimer::timeout, this, &buffer_test::testExtractBufferData);
}
buffer_test::~buffer_test() {

}
void buffer_test::start_timer(){
    m_buffer_test_timer->start(1000);
}
void buffer_test::stop_timer(){
    m_buffer_test_timer->stop();
}

void buffer_test::testExtractBufferData() {
    int time = 200;
    int src_id = 1;
    int dest_id = 2;
    int len = 50;

    std::stringstream ss;
    ss << time << "," << src_id << "," << dest_id << "," << len << ",";

    char buffer[1024];
    size_t offset = 0;

// int32_t
    int32_t testInt32 = -12345;
    memcpy(buffer + offset, &testInt32, sizeof(testInt32));
    offset += sizeof(testInt32);

// uint32_t
    uint32_t testUint32 = 54321;
    memcpy(buffer + offset, &testUint32, sizeof(testUint32));
    offset += sizeof(testUint32);

// char*
    const char *testString = "Hello, World!";
    memcpy(buffer + offset, testString, 14); // Including null terminator
    offset += 14;

// float
    float testFloat = 3.14159f;
    memcpy(buffer + offset, &testFloat, sizeof(float));
    offset += sizeof(float);

// double
    double testDouble = 2.71828182845904;
    memcpy(buffer + offset, &testDouble, sizeof(double));
    offset += sizeof(double);

// bool (3 booleans packed into a uint8_t)
    uint8_t testBools = 0b00000101; // true, false, true
    memcpy(buffer + offset, &testBools, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < offset; ++i) {
        ss << std::setw(2) << static_cast<int>(static_cast<unsigned char>(buffer[i]));
    }

    QString qstr = QString::fromStdString(ss.str());

    GlobalState::getInstance().newData(qstr);
}