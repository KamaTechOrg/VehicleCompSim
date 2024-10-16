//
// Created by OWNER on 22/09/2024.
//

#include <QFileDialog>
#include "saveAndLoad.h"

saveAndLoad::saveAndLoad(GlobalState *globalState) : m_globalState(globalState){
        connect(&GlobalState::getInstance(), &GlobalState::saveBtnPressed, this, &saveAndLoad::saveLayout);
        connect(&GlobalState::getInstance(), &GlobalState::loadBtnPressed, this, &saveAndLoad::loadLayout);
}

void saveAndLoad::bson_to_qemu(const bson_t *bsonDocument) {
    auto *qemuModel = new QemuSensorModel();
    bson_iter_t iter;
    bson_iter_init(&iter, bsonDocument);
    bson_iter_next(&iter);
    bson_iter_next(&iter);
    qemuModel->setOwnerID(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setId(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setX(bson_iter_double(&iter));
    bson_iter_next(&iter);
    qemuModel->setY(bson_iter_double(&iter));
    bson_iter_next(&iter);
    qemuModel->setPriority(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setName(bson_iter_utf8(&iter, nullptr));
    // qemu info
    bson_iter_next(&iter);
    qemuModel->setPlatform(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setMachine(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setCpu(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setMemory_MB(bson_iter_int64(&iter));
    bson_iter_next(&iter);
    qemuModel->setKernal(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setHarddrive(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setCdrom(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setBoot(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setNet(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setAppend(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    qemuModel->setNographic(bson_iter_bool(&iter));
    m_globalState->currentProject()->addModel(qemuModel);
}

void saveAndLoad::bson_to_sensor(const bson_t *bsonDocument) {
    SensorModel* sensorModel = new SensorModel();
    bson_iter_t iter;
    bson_iter_init(&iter, bsonDocument);
    bson_iter_next(&iter);
    bson_iter_next(&iter);
    sensorModel->setOwnerID(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    sensorModel->setId(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    sensorModel->setX(bson_iter_double(&iter));
    bson_iter_next(&iter);
    sensorModel->setY(bson_iter_double(&iter));
    bson_iter_next(&iter);
    sensorModel->setPriority(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    sensorModel->setName(bson_iter_utf8(&iter, nullptr));
    // sensor info
    bson_iter_next(&iter);
    sensorModel->setBuildCommand(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    sensorModel->setRunCommand(bson_iter_utf8(&iter, nullptr));
    bson_iter_next(&iter);
    sensorModel->setCmakePath(bson_iter_utf8(&iter, nullptr));
    m_globalState->currentProject()->addModel(sensorModel);
}

void saveAndLoad::loadLayout(const QString &dirPath) {
    QString path;
    if(!dirPath.isEmpty()){
        path = dirPath;
    }else{
        path = QFileDialog::getOpenFileName(nullptr, tr("Select BSON File"), QString(),
                                                                tr("BSON Files (*.bson);;All Files (*)"));
    }
    auto models = m_globalState->currentProject()->models();
    for (auto model : models) {
        m_globalState->currentProject()->removeModel(model);
    }
    if (!path.isEmpty()) {
        bson_reader_t* reader = bson_reader_new_from_file(path.toUtf8().constData(), NULL);
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
        GlobalState::getInstance().log("File selection canceled by the user.", "Terminal");
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
    BSON_APPEND_UTF8(base_BSON, "ownerId", qemu->ownerID().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "id", qemu->getId().toUtf8().constData());
    BSON_APPEND_DOUBLE(base_BSON, "pos_x", qemu->x());
    BSON_APPEND_DOUBLE(base_BSON, "pos_y", qemu->y());
    BSON_APPEND_UTF8(base_BSON, "priority", qemu->priority().toUtf8().constData());
    BSON_APPEND_UTF8(base_BSON, "name", qemu->name().toUtf8().constData());
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

void saveAndLoad::SaveBsonToFile(std::vector<bson_t*> &bson_obj_vector, const QString &dirPath) {
    QString path;
    if(!dirPath.isEmpty()){
        path = dirPath;
    }else{
        QString defaultFileName = "layout.bson";
        path = QFileDialog::getSaveFileName(nullptr, "Save BSON File", defaultFileName, "BSON Files (*.bson);;All Files (*)");
    }
    if (!path.isEmpty()) {
        QFile outputFile(path);
        if (outputFile.open(QIODevice::WriteOnly)) {
            for(bson_t * bson_obj : bson_obj_vector) {
                const uint8_t *bsonData = bson_get_data(bson_obj);
                outputFile.write(reinterpret_cast<const char *>(bsonData), bson_obj->len);
            }
            outputFile.close();
        }
    }
}

void saveAndLoad::saveLayout(const QString &dirPath) {
    std::vector<bson_t*> base_BSON_vector;
    auto models = m_globalState->currentProject()->models();
    for (auto model: models) {
        if (auto *qemu = dynamic_cast<QemuSensorModel *>(model)) {
            base_BSON_vector.push_back(qemu_to_bson(dynamic_cast<QemuSensorModel *>(qemu)));
        }else if (auto *sensor = dynamic_cast<SensorModel *>(model)) {
            base_BSON_vector.push_back(sensor_to_bson(dynamic_cast<SensorModel *>(sensor)));
        }
    }
    SaveBsonToFile(base_BSON_vector, dirPath);
}
