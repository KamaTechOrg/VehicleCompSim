//
// Created by OWNER on 22/09/2024.
//

#include <QFileDialog>
#include "saveAndLoad.h"
#include <QWidget>


saveAndLoad::saveAndLoad(GlobalState *globalState) : m_globalState(globalState){
        connect(&GlobalState::getInstance(), &GlobalState::saveBtnPressed, this, &saveAndLoad::saveLayout);
        connect(&GlobalState::getInstance(), &GlobalState::loadBtnPressed, this, &saveAndLoad::loadLayout);
}

void saveAndLoad::bson_to_qemu(const bson_t *bsonDocument) {
    auto *new_qemu = new QemuSensorModel();
    bson_iter_t iter;
    bson_iter_init(&iter, bsonDocument);
    bson_iter_next(&iter);
    // location
    bson_iter_next(&iter);
    new_qemu->setX(bson_iter_double(&iter));
    bson_iter_next(&iter);
    new_qemu->setY(bson_iter_double(&iter));
    // qemu id name
    bson_iter_next(&iter);
    new_qemu->setPriority(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_qemu->setName(bson_iter_utf8(&iter, nullptr));
    // qemu info
    bson_iter_next(&iter);
    new_qemu->setPlatform(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_qemu->setMachine(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_qemu->setCpu(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_qemu->setMemory_MB(bson_iter_int64(&iter));
    bson_iter_next(&iter);
    new_qemu->setKernal(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_qemu->setHarddrive(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_qemu->setCdrom(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_qemu->setBoot(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_qemu->setNet(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_qemu->setAppend(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_qemu->setNographic(bson_iter_bool(&iter));
    m_globalState->currentProject()->addModel(new_qemu);
}
// todo check the creation of items after the update to model type
//void ssss(){
//    if (itemType == CustomWidget::REGULAR_SENSOR_ITEM) {
//        SensorModel* sensorModel = new SensorModel();
//        sensorModel->setOwnerID(m_globalState.myClientId());
//        SensorItem* sensorItem = new SensorItem(sensorModel);
//        sensorItem->setPos(event->scenePos());
//        m_network->addElement(sensorItem);
//        addItemToScene(sensorItem);
//        m_globalState.setCurrentSensorModel(sensorModel);
//    }
//    else if (itemType == CustomWidget::QEMU_SENSOR_ITEM) {
//        QemuSensorModel* qemuModel = new QemuSensorModel();
//        qemuModel->setOwnerID(m_globalState.myClientId());
//        QemuSensorItem* qemuItem = new QemuSensorItem(qemuModel);
//        qemuItem->setPos(event->scenePos());
//        m_network->addElement(qemuItem);
//        addItemToScene(qemuItem);
//        m_globalState.setCurrentSensorModel(qemuModel);
//    }
//}

void saveAndLoad::bson_to_sensor(const bson_t *bsonDocument) {
    auto *new_sensor = new SensorModel();
    bson_iter_t iter;
    bson_iter_init(&iter, bsonDocument);
    bson_iter_next(&iter);
    bson_iter_next(&iter);
    new_sensor->setOwnerID(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setId(bson_iter_utf8(&iter, nullptr));
    // location
    bson_iter_next(&iter);
    new_sensor->setX(bson_iter_double(&iter));
    bson_iter_next(&iter);
    new_sensor->setY(bson_iter_double(&iter));
    // sensor info
    bson_iter_next(&iter);
    new_sensor->setPriority(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setName(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setBuildCommand(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setRunCommand(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    new_sensor->setCmakePath(bson_iter_utf8(&iter, nullptr));
    m_globalState->currentProject()->addModel(new_sensor);
}

void saveAndLoad::loadLayout() {
    auto models = m_globalState->currentProject()->models();
    for (auto model : models) {
        m_globalState->currentProject()->removeModel(model);
    }
    QString selectedFileName = QFileDialog::getOpenFileName(this, tr("Select BSON File"), QString(),
                                                            tr("BSON Files (*.bson);;All Files (*)"));
    if (!selectedFileName.isEmpty()) {
        bson_reader_t* reader = bson_reader_new_from_file(selectedFileName.toUtf8().constData(), NULL);
        const bson_t* bsonDocument;
        while ((bsonDocument = bson_reader_read(reader, NULL))) {
            bson_iter_t iter;
            bson_iter_init(&iter, bsonDocument);
            bson_iter_next(&iter);
            QString type = bson_iter_utf8(&iter, nullptr);
            if (type == "Qemu") {
                bson_to_qemu(bsonDocument);
            }
            if (type == "Sensor") {
                bson_to_sensor(bsonDocument);
            }
        }
        bson_reader_destroy(reader);
    } else {
        qInfo() << "File selection canceled by the user.";
    }
}

bson_t* saveAndLoad::sensor_to_bson(SensorModel* sensor) {
    bson_t* base_BSON = bson_new();
    BSON_APPEND_UTF8(base_BSON, "type", "Sensor");
    BSON_APPEND_UTF8(base_BSON, "ownerId", sensor->ownerID().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "id", sensor->getId().toUtf8().constData());
    BSON_APPEND_DOUBLE(base_BSON, "pos_x", sensor->x());
    BSON_APPEND_DOUBLE(base_BSON, "pos_y", sensor->y());
    BSON_APPEND_UTF8(base_BSON, "priority", sensor->priority().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "name", sensor->name().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "buildCommand", sensor->buildCommand().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "runCommand", sensor->runCommand().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "cmakePath", sensor->cmakePath().toUtf8().constData());
    return base_BSON;
}
bson_t* saveAndLoad::qemu_to_bson(QemuSensorModel* qemu){
    bson_t* base_BSON = bson_new();
    BSON_APPEND_UTF8(base_BSON, "type", "Qemu");
    BSON_APPEND_UTF8(base_BSON, "priority", qemu->priority().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "name", qemu->name().toUtf8().constData());
    BSON_APPEND_DOUBLE(base_BSON, "pos_x", qemu->x());
    BSON_APPEND_DOUBLE(base_BSON, "pos_y", qemu->y());
    BSON_APPEND_UTF8(base_BSON, "platform", qemu->platform().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "machine", qemu->machine().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "cpu", qemu->cpu().toUtf8().constData());
    BSON_APPEND_INT64(base_BSON, "m_memory_MB", qemu->memory_MB());
    BSON_APPEND_UTF8(base_BSON, "kernal", qemu->kernal().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "harddrive", qemu->harddrive().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "cdrom", qemu->cdrom().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "boot", qemu->boot().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "m_net", qemu->net().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "m_append", qemu->append().toUtf8().constData());
    BSON_APPEND_BOOL(base_BSON, "m_nographic", qemu->nographic());
    return base_BSON;
}

void saveAndLoad::SaveBsonToFile(std::vector<bson_t*> &bson_obj_vector) {
    QString defaultFileName = "layout.bson";
    QString selectedFileName = QFileDialog::getSaveFileName(nullptr, "Save BSON File", defaultFileName, "BSON Files (*.bson);;All Files (*)");
    if (!selectedFileName.isEmpty()) {
        QFile outputFile(selectedFileName);
        if (outputFile.open(QIODevice::WriteOnly)) {
            for(bson_t * bson_obj : bson_obj_vector) {
                const uint8_t *bsonData = bson_get_data(bson_obj);
                outputFile.write(reinterpret_cast<const char *>(bsonData), bson_obj->len);
            }
            outputFile.close();
        }
    }
}

void saveAndLoad::saveLayout() {
    std::vector<bson_t*> base_BSON_vector;
    auto models = m_globalState->currentProject()->models();
    for (auto model: models) {
        if (auto *qemu = dynamic_cast<QemuSensorModel *>(model)) {
            base_BSON_vector.push_back(qemu_to_bson(dynamic_cast<QemuSensorModel *>(qemu)));
        }else if (auto *sensor = dynamic_cast<SensorModel *>(model)) {
            base_BSON_vector.push_back(sensor_to_bson(dynamic_cast<SensorModel *>(sensor)));
        }
    }
    SaveBsonToFile(base_BSON_vector);
}
