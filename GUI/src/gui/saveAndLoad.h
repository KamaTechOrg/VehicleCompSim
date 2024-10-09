//
// Created by OWNER on 22/09/2024.
//

#ifndef MYPROJECT_SAVEANDLOAD_H
#define MYPROJECT_SAVEANDLOAD_H

#include "bson/bson.h"
#include "models/sensormodel.h"
#include "state/globalstate.h"
#include "models/qemusensormodel.h"

class saveAndLoad :public QWidget{
Q_OBJECT

public:
    saveAndLoad(GlobalState *globalState);
    void bson_to_sensor(const bson_t *bsonDocument);
    void loadLayout();
    bson_t* sensor_to_bson(SensorModel* sensor);
    void SaveBsonToFile(std::vector<bson_t*> &bson_obj_vector);
    void saveLayout();
    bson_t* qemu_to_bson(QemuSensorModel* qemu);
    void bson_to_qemu(const bson_t *bsonDocument);

private:
    GlobalState *m_globalState;

    };


#endif //MYPROJECT_SAVEANDLOAD_H
