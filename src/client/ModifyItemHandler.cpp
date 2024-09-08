#include "ModifyItemHandler.h"
#include "items/connectoritem.h"
#include "items/edgeitem.h"
#include "items/sensoritem.h"
#include <QDebug>

ModifyItemHandler::ModifyItemHandler(CustomScene* scene)
    : m_scene(scene) {}

void ModifyItemHandler::handle(const QJsonObject& message) {
    SerializableItem item;
    item.deserialize(message);

    int itemType = message["type"].toInt();
    QGraphicsItem* graphicsItem = createGraphicsItem(itemType, message);
    if (graphicsItem) {
        m_scene->modifyItem(graphicsItem);
        qDebug() << "Item modified in scene";
    }
}

QGraphicsItem* ModifyItemHandler::createGraphicsItem(int itemType, const QJsonObject& jsonObj) {
    if (itemType == static_cast<int>(ItemType::Connector)) {
        auto temp = new ConnectorItem();
        temp->deserialize(jsonObj);
        return temp;
    } else if (itemType == static_cast<int>(ItemType::Edge)) {
        auto temp = new EdgeItem();
        temp->deserialize(jsonObj);
        return temp;
    } else if (itemType == static_cast<int>(ItemType::Sensor)) {
        auto temp = new SensorItem();
        temp->deserialize(jsonObj);
        return temp;
    }
    return nullptr;
}
