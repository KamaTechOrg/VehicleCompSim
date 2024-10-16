#include <gtest/gtest.h>
#include "parser.h"

class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        p = new parser();
    }

    void TearDown() override {
        delete p;
    }

    parser* p;
};

TEST_F(ParserTest, ParseBuffer) {
    QString buffer = "123,1,2,4,deadbeef";
    size_t bufferSize = buffer.size();

    // We can't directly verify the result of parseBuffer as it doesn't return anything
    // and we don't have access to GlobalState in this test.
    // Instead, we'll just verify that it doesn't crash.
    EXPECT_NO_THROW(p->parseBuffer(buffer, bufferSize));
}

TEST_F(ParserTest, SetSensorInfoMap) {
    QMap<int, QList<QList<QString>>> testMap;
    QList<QList<QString>> info = {{"name", "4", "int32_t"}};
    testMap.insert(1, info);

    // We can't directly verify the result of setSensorInfoMap as it doesn't return anything
    // and we don't have access to the private member sensorInfoMap.
    // Instead, we'll just verify that it doesn't crash.
    EXPECT_NO_THROW(p->setSensorInfoMap(testMap));
}

TEST_F(ParserTest, ExtractBufferData) {
    QByteArray buffer;
    buffer.append(static_cast<char>(0x04)); // int32_t value 4 in little-endian
    buffer.append(static_cast<char>(0x00));
    buffer.append(static_cast<char>(0x00));
    buffer.append(static_cast<char>(0x00));
    size_t bufferSize = buffer.size();
    QList<QList<QString>> columnInfo = {{"name", "4", "int32_t"}};

    QList<QPair<QString, QString>> result = p->extractBufferData(buffer, bufferSize, columnInfo);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].first, "name");
    EXPECT_EQ(result[0].second, "4");
}

TEST_F(ParserTest, ExtractBufferDataMultipleColumns) {
    QByteArray buffer;
    // int32_t value 4
    buffer.append(static_cast<char>(0x04));
    buffer.append(static_cast<char>(0x00));
    buffer.append(static_cast<char>(0x00));
    buffer.append(static_cast<char>(0x00));
    // uint32_t value 5
    buffer.append(static_cast<char>(0x05));
    buffer.append(static_cast<char>(0x00));
    buffer.append(static_cast<char>(0x00));
    buffer.append(static_cast<char>(0x00));
    // float value 3.14
    union { float f; char bytes[4]; } u;
    u.f = 3.14f;
    buffer.append(u.bytes[0]);
    buffer.append(u.bytes[1]);
    buffer.append(u.bytes[2]);
    buffer.append(u.bytes[3]);
    
    size_t bufferSize = buffer.size();
    QList<QList<QString>> columnInfo = {
        {"int_val", "4", "int32_t"},
        {"uint_val", "4", "uint32_t"},
        {"float_val", "4", "float"}
    };

    QList<QPair<QString, QString>> result = p->extractBufferData(buffer, bufferSize, columnInfo);
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0].first, "int_val");
    EXPECT_EQ(result[0].second, "4");
    EXPECT_EQ(result[1].first, "uint_val");
    EXPECT_EQ(result[1].second, "5");
    EXPECT_EQ(result[2].first, "float_val");
    EXPECT_NEAR(result[2].second.toFloat(), 3.14f, 0.001f);
}

TEST_F(ParserTest, ExtractBufferDataEmptyBuffer) {
    QByteArray buffer;
    size_t bufferSize = 0;
    QList<QList<QString>> columnInfo = {{"name", "4", "int32_t"}};

    QList<QPair<QString, QString>> result = p->extractBufferData(buffer, bufferSize, columnInfo);
    EXPECT_TRUE(result.isEmpty());
}

TEST_F(ParserTest, ExtractBufferDataInvalidType) {
    QByteArray buffer;
    buffer.append(static_cast<char>(0x04));
    buffer.append(static_cast<char>(0x00));
    buffer.append(static_cast<char>(0x00));
    buffer.append(static_cast<char>(0x00));
    size_t bufferSize = buffer.size();
    QList<QList<QString>> columnInfo = {{"name", "4", "invalid_type"}};

    QList<QPair<QString, QString>> result = p->extractBufferData(buffer, bufferSize, columnInfo);
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].first, "name");
    EXPECT_EQ(result[0].second, "Unsupported type");
}