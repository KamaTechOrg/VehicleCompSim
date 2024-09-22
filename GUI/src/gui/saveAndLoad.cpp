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


void saveAndLoad::create_sensor_from_bson_obj(const bson_t *bsonDocument) {
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
    double x = bson_iter_double(&iter);
    bson_iter_next(&iter);
    double y = bson_iter_double(&iter);
    new_sensor->setX(x);
    new_sensor->setY(y);
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
        delete model; // Properly delete the model
    }
//    m_scene->clear();
    QString selectedFileName = QFileDialog::getOpenFileName(this, tr("Select BSON File"), QString(),
                                                            tr("BSON Files (*.bson);;All Files (*)"));
    if (!selectedFileName.isEmpty()) {
        bson_reader_t* reader = bson_reader_new_from_file(selectedFileName.toUtf8().constData(), NULL);
        const bson_t* bsonDocument;
        while ((bsonDocument = bson_reader_read(reader, NULL))) {
            create_sensor_from_bson_obj(bsonDocument);
        }
        bson_reader_destroy(reader);
    } else {
        qInfo() << "File selection canceled by the user.";
    }
}

bson_t* saveAndLoad::sensor_to_bson_obj(SensorModel* sensor) {
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
        if (auto *sensor = dynamic_cast<SensorModel *>(model)) {
            base_BSON_vector.push_back(sensor_to_bson_obj(dynamic_cast<SensorModel*>(sensor)));
        }
    }
    SaveBsonToFile(base_BSON_vector);
}
