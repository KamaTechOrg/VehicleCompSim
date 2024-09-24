//
// Created by OWNER on 22/09/2024.
//

#ifndef MYPROJECT_SAVEANDLOAD_H
#define MYPROJECT_SAVEANDLOAD_H

#include "bson/bson.h"
#include "models/sensormodel.h"
#include "state/globalstate.h"


class saveAndLoad :public QWidget{
Q_OBJECT

public:
    saveAndLoad(GlobalState *globalState);
    void create_sensor_from_bson_obj(const bson_t *bsonDocument);
    void loadLayout();
    bson_t* sensor_to_bson_obj(SensorModel* sensor);
    void SaveBsonToFile(std::vector<bson_t*> &bson_obj_vector);
    void saveLayout();
private:
    GlobalState *m_globalState;

    };


#endif //MYPROJECT_SAVEANDLOAD_H
