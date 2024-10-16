// #include <gtest/gtest.h>
// #include "saveAndLoad.h"
// #include "state/globalstate.h"
// #include "models/sensormodel.h"
// #include "models/qemusensormodel.h"

// class SaveAndLoadTest : public ::testing::Test {
// protected:
//     GlobalState * globalState = &GlobalState::getInstance();
//     saveAndLoad sl;

//     SaveAndLoadTest() : sl(globalState) {}

//     void SetUp() override {
//         if(globalState->currentProject() == nullptr){
//             globalState->setCurrentProject(new ProjectModel("testProject"));
//         }
//     }

//     void TearDown() override {
//         // Cleanup code if needed
//     }
// };

// TEST_F(SaveAndLoadTest, BsonToQemu) {
//     bson_t* bsonDocument = BCON_NEW(
//         "type", BCON_UTF8("Qemu"),
//         "ownerId", BCON_UTF8("owner1"),
//         "id", BCON_UTF8("id1"),
//         "pos_x", BCON_DOUBLE(1.0),
//         "pos_y", BCON_DOUBLE(2.0),
//         "priority", BCON_UTF8("high"),
//         "name", BCON_UTF8("qemu1"),
//         "platform", BCON_UTF8("x86"),
//         "machine", BCON_UTF8("pc"),
//         "cpu", BCON_UTF8("i7"),
//         "m_memory_MB", BCON_INT64(4096),
//         "kernal", BCON_UTF8("vmlinuz"),
//         "harddrive", BCON_UTF8("sda"),
//         "cdrom", BCON_UTF8("cdrom"),
//         "boot", BCON_UTF8("boot"),
//         "m_net", BCON_UTF8("net"),
//         "m_append", BCON_UTF8("append"),
//         "m_nographic", BCON_BOOL(true)
//     );

//     sl.bson_to_qemu(bsonDocument);
//     // Add assertions based on expected behavior
// }

// TEST_F(SaveAndLoadTest, BsonToSensor) {
//     bson_t* bsonDocument = BCON_NEW(
//         "type", BCON_UTF8("Sensor"),
//         "ownerId", BCON_UTF8("owner1"),
//         "id", BCON_UTF8("id1"),
//         "pos_x", BCON_DOUBLE(1.0),
//         "pos_y", BCON_DOUBLE(2.0),
//         "priority", BCON_UTF8("high"),
//         "name", BCON_UTF8("sensor1"),
//         "buildCommand", BCON_UTF8("build"),
//         "runCommand", BCON_UTF8("run"),
//         "cmakePath", BCON_UTF8("path")
//     );

//     sl.bson_to_sensor(bsonDocument);
//     // Add assertions based on expected behavior
// }

// TEST_F(SaveAndLoadTest, SensorToBson) {
//     SensorModel sensor;
//     sensor.setId("id1");
//     sensor.setName("sensor1");
//     sensor.setBuildCommand("build");
//     sensor.setRunCommand("run");
//     sensor.setCmakePath("path");

//     bson_t* bsonDocument = sl.sensor_to_bson(&sensor);
//     // Add assertions based on expected behavior
// }

// TEST_F(SaveAndLoadTest, QemuToBson) {
//     QemuSensorModel qemu;
//     qemu.setId("id1");
//     qemu.setName("qemu1");
//     qemu.setPlatform("x86");
//     qemu.setMachine("pc");
//     qemu.setCpu("i7");
//     qemu.setMemory_MB(4096);
//     qemu.setKernal("vmlinuz");
//     qemu.setHarddrive("sda");
//     qemu.setCdrom("cdrom");
//     qemu.setBoot("boot");
//     qemu.setNet("net");
//     qemu.setAppend("append");
//     qemu.setNographic(true);

//     bson_t* bsonDocument = sl.qemu_to_bson(&qemu);
//     // Add assertions based on expected behavior
// }

// TEST_F(SaveAndLoadTest, SaveBsonToFile) {
//     std::vector<bson_t*> bson_obj_vector;
//     bson_t* bsonDocument = BCON_NEW(
//         "type", BCON_UTF8("Qemu"),
//         "ownerId", BCON_UTF8("owner1"),
//         "id", BCON_UTF8("id1")
//     );
//     bson_obj_vector.push_back(bsonDocument);

//     sl.SaveBsonToFile(bson_obj_vector);
//     // Add assertions based on expected behavior
// }

// TEST_F(SaveAndLoadTest, LoadLayout) {
//     sl.loadLayout();
//     // Add assertions based on expected behavior
// }

// TEST_F(SaveAndLoadTest, SaveLayout) {
//     sl.saveLayout();
//     // Add assertions based on expected behavior
// }