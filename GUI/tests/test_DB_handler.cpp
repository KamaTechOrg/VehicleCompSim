#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>
#include "DB_handler.h"
#include "state/globalstate.h"

class DBHandlerTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        int argc = 0;
        char* argv[] = {nullptr};
        app = new QCoreApplication(argc, argv);
    }

    static void TearDownTestSuite() {
        delete app;
    }

    void SetUp() override {
        dbHandler = new DB_handler();
    }

    void TearDown() override {
        delete dbHandler;
    }

    static QCoreApplication* app;
    DB_handler* dbHandler;
};

QCoreApplication* DBHandlerTest::app = nullptr;

TEST_F(DBHandlerTest, WriteDataToDB) {
    QString data = "123,1,2,4,deadbeef";
    size_t bufferSize = data.size();

    // dbHandler->write_data_to_DB(data);

    // Verify that the data is correctly written to the database
    QSqlQuery query(*dbHandler->sqlitedb);
    ASSERT_TRUE(query.exec("SELECT * FROM DB"));
    ASSERT_TRUE(query.next());

    EXPECT_EQ(query.value(0).toString(), "123");
    EXPECT_EQ(query.value(1).toString(), "1");
    EXPECT_EQ(query.value(2).toString(), "2");
    EXPECT_EQ(query.value(3).toString(), "4");
    EXPECT_EQ(query.value(4).toString(), "deadbeef");
}

TEST_F(DBHandlerTest, UpdateSensorData) {
    wint_t sensorId = 1;
    QList<QList<QString>> data = {{"name", "4", "int32_t"}};

    dbHandler->update_sensor_data(sensorId, data);

    // Verify that the sensor data is correctly updated
    ASSERT_TRUE(dbHandler->data_of_sensors.contains(sensorId));
    EXPECT_EQ(dbHandler->data_of_sensors[sensorId], data);
}
