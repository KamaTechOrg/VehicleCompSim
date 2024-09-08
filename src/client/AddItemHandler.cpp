#include "AddItemHandler.h"
#include "items/connectoritem.h"
#include "items/edgeitem.h"
#include "items/sensoritem.h"
#include <QDebug>

AddItemHandler::AddItemHandler(CustomScene* scene)
    : m_scene(scene) {}

void AddItemHandler::handle(const QJsonObject& message) {
    SerializableItem item;
    item.deserialize(message);

    int itemType = message["type"].toInt();
    QGraphicsItem* graphicsItem = createGraphicsItem(itemType, message);
    if (graphicsItem) {
        m_scene->addItem(graphicsItem);
        qDebug() << "Item added to scene";
    }
}

QGraphicsItem* AddItemHandler::createGraphicsItem(int itemType, const QJsonObject& jsonObj) {
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
