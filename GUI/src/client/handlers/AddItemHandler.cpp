#include "AddItemHandler.h"
#include "maincomputermodel.h"
#include "qemusensormodel.h"
#include "serializableitem.h"
#include "sensorModel.h"
#include <QDebug>
#include "clientconstants.h"

void AddItemHandler::handle(const QJsonObject& message) {
    QString projectId = message[ClientConstants::KEY_PROJECT_ID].toString();
    if(projectId != GlobalState::getInstance().currentProject()->id()) {
        qDebug() << "The recieved item is not for the current project";
        return;
    }
    ItemType itemType = static_cast<ItemType>(message[ClientConstants::KEY_TYPE].toInt());
    SerializableItem* model = createModel(itemType, message);
    if (model) {
        ProjectModel* currentProject = GlobalState::getInstance().currentProject();
        if (currentProject) {
            currentProject->addModel(model);
            qDebug() << "Model added to project";
        }
    }
}

SerializableItem* AddItemHandler::createModel(ItemType itemType, const QJsonObject& jsonObj) {
    SerializableItem* item = nullptr;

    if (itemType == ItemType::Sensor) {
        item = new SensorModel;
    } else     if (itemType == ItemType::Qemu) {
        item = new QemuSensorModel;
    } else    if (itemType == ItemType::MainComputer) {
        item = new MainComputerModel;
    } else  if (itemType == ItemType::Connector) {
        auto model = new SerializableItem(ItemType::Connector);
        model->deserialize(jsonObj);
        return model;
    // } else if (itemType == static_cast<int>(ItemType::Edge)) { TODO: 
    //     auto temp = new EdgeItem();
    //     temp->deserialize(jsonObj);
    //     return temp;
    }
    if (item) item->deserialize(jsonObj);
    return item;
}
