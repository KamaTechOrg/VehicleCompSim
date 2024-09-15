#include "customscene.h"
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "items/sensoritem.h"
#include "items/connectoritem.h"

CustomScene::CustomScene(QObject* parent)
    : QGraphicsScene(parent), m_network(new Network<SensorItem, ConnectorItem>()) {

}

void CustomScene::addItemToScene(SerializableItem *item)
{
    if(item == nullptr){
        return;
    }
    BaseItem* baseItem = dynamic_cast<BaseItem*>(item);
    if(baseItem){
        addItem(baseItem);
    } else
    {
        EdgeItem* edgeItem = dynamic_cast<EdgeItem*>(item);
        if(edgeItem){
            addItem(edgeItem);
        }
    }
    item->notifyItemAdded();
}

void CustomScene::removeItemFromScene(SerializableItem *item)
{
    if(item == nullptr){
        return;
    }
    item->notifyItemDeleted();
    BaseItem* baseItem = dynamic_cast<BaseItem*>(item);
    if(baseItem){
        removeItem(baseItem);
    } else
    {
        EdgeItem* edgeItem = dynamic_cast<EdgeItem*>(item);
        if(edgeItem){
            removeItem(edgeItem);
        }
    }
}

void CustomScene::modifyItem(QGraphicsItem *item)
{
    if(item){
        SerializableItem* serializableItem = dynamic_cast<SerializableItem*>(item);
        for(auto itemInScene: items()){
            auto serializableItemInScene = dynamic_cast<SerializableItem*>(itemInScene);
            if(serializableItemInScene && serializableItemInScene->getId() == serializableItem->getId()){
                serializableItemInScene->deserialize(serializableItem->serialize());
                break;
            } 
        }
    }
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QList<QGraphicsItem*> itemsAtClick = items(event->scenePos());
        BaseItem* baseItem = nullptr;

        for (QGraphicsItem* item : itemsAtClick) {
            if (dynamic_cast<EdgeItem*>(item)) {
                continue;
            }
            baseItem = dynamic_cast<BaseItem*>(item);
            if (baseItem) {
                break;
            }
        }
        if (baseItem) {
            processBaseItem(event, baseItem);
        } else {
            cleanupCurrentEdge();
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void CustomScene::processBaseItem(QGraphicsSceneMouseEvent* event, BaseItem* baseItem) {
    if (baseItem->isNearConnectionPoint(event->scenePos())) {
        if (m_currentEdge != nullptr) {
            handleEdgeConnection(event, baseItem);
        } else {
            startNewEdge(event, baseItem);
        }
    } else {
        cleanupCurrentEdge();
    }
}

void CustomScene::handleEdgeConnection(QGraphicsSceneMouseEvent* event, BaseItem* baseItem) {
    if (baseItem != m_startItem && baseItem->isNearConnectionPoint(event->scenePos())) {
        // m_currentEdge->setDest(baseItem);
        // routeEdge(m_currentEdge);
        buildConnection(m_startItem, baseItem);
    } else {
        removeItem(m_currentEdge);
        delete m_currentEdge;
    }
    m_currentEdge = nullptr;
    m_startItem = nullptr;
}

void CustomScene::startNewEdge(QGraphicsSceneMouseEvent* event, BaseItem* baseItem) {
    m_startItem = baseItem;
    m_startPoint = baseItem->connectionPoint(event->scenePos());
    m_currentEdge = new EdgeItem();
    addItem(m_currentEdge);
    m_currentEdge->setSource(m_startItem);

    QPainterPath path;
    path.moveTo(m_startPoint);
    path.lineTo(m_startPoint); // Initially, the line is just a point

    m_currentEdge->setPath(path);
}

void CustomScene::cleanupCurrentEdge() {
    if (m_currentEdge) {
        removeItem(m_currentEdge);
        delete m_currentEdge;
        m_currentEdge = nullptr;
    }
    m_startItem = nullptr;
}

void CustomScene::buildConnection(BaseItem *src, BaseItem *dest)
{
    if(src->itemType() == ItemType::Connector){
        if(dest->itemType() == ItemType::Connector){
            removeItemFromScene(m_currentEdge);
            delete m_currentEdge;
            return;            
        }
        else{
            std::swap(src, dest);            
        }
    }
    QPainterPath path;
    QPointF startPoint = src->connectionPoint(m_startPoint);
    QPointF endPoint = dest->connectionPoint(m_startPoint);
    if(dest->itemType() == ItemType::Connector){
        QPointF midPoint = (startPoint + endPoint) / 2;
        QPointF controlPoint1 = midPoint + QPointF(-50, -50); 
        QPointF controlPoint2 = midPoint + QPointF(50, 50); 

        path.moveTo(startPoint);
        path.cubicTo(controlPoint1, controlPoint2, endPoint);

        EdgeItem* edge = new EdgeItem();
        edge->setPath(path);
        edge->setSource(src);
        edge->setDest(dest);

        addItemToScene(edge);
        src->addEdge(edge);
        dest->addEdge(edge);

        m_network->connect(dynamic_cast<SensorItem*>(src), dynamic_cast<ConnectorItem*>(dest));
    } else if(dest->itemType() == ItemType::Sensor){
        //create a new connector in the middle of the two sensors
        QPointF middlePoint = (startPoint + endPoint) / 2;
        ConnectorItem* connector = new ConnectorItem();
        connector->setPos(middlePoint);

        QPointF connectorPointIn = connector->connectionPoint(startPoint);
        QPointF connectorPointOut = connector->connectionPoint(endPoint);

        QPointF midPoint = (startPoint + connectorPointIn) / 2;
        QPointF controlPoint1 = midPoint + QPointF(-50, -50); 
        QPointF controlPoint2 = midPoint + QPointF(50, 50); 

        path.moveTo(startPoint);
        path.cubicTo(controlPoint1, controlPoint2, connectorPointIn);

        EdgeItem* edge1 = new EdgeItem();
        edge1->setPath(path);
        edge1->setSource(src);
        edge1->setDest(connector);
        src->addEdge(edge1);
        connector->addEdge(edge1);

        path = QPainterPath();
        
        midPoint = (connectorPointOut + endPoint) / 2;
        controlPoint1 = midPoint + QPointF(-50, -50);
        controlPoint2 = midPoint + QPointF(50, 50);

        path.moveTo(connectorPointOut);
        path.cubicTo(controlPoint1, controlPoint2, endPoint);

        EdgeItem* edge2 = new EdgeItem();
        edge2->setPath(path);
        edge2->setSource(connector);
        edge2->setDest(dest);
        connector->addEdge(edge2);
        dest->addEdge(edge2);

        addItemToScene(connector);
        addItemToScene(edge1);
        addItemToScene(edge2);

        m_network->connect(dynamic_cast<SensorItem*>(src), connector);
        m_network->connect(dynamic_cast<SensorItem*>(dest), connector);
    }

    delete m_currentEdge;
    m_currentEdge = nullptr;
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_currentEdge) {
        QPointF endPoint = event->scenePos();
        QLineF line(m_startPoint, endPoint);
        
        QPainterPath path;
        path.moveTo(line.p1());
        path.lineTo(line.p2());
        
        m_currentEdge->setPath(path);
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void CustomScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText()) {
        event->setAccepted(true);
    } else {
        event->setAccepted(false);
    }
}

void CustomScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText()) {
        event->setAccepted(true);
    } else {
        event->setAccepted(false);
    }
}

void CustomScene::dropEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText()) {
        QString itemType = event->mimeData()->text();
        BaseItem* item = nullptr;
        if (itemType == "SensorItem") {
            SensorItem* sensorItem = new SensorItem();
            sensorItem->popupDialog = popupDialog;
            item = sensorItem;
            m_network->addElement(dynamic_cast<SensorItem*>(item));
            //popupDialog->oldSensorItem = sensorItem;
            //popupDialog->reset();
            // Handle cancellation
//            if (popupDialog->cancel) {
//                // Remove the sensorItem from the network
//                m_network->removeElement(sensorItem);
//                delete sensorItem;
//                return;
//            }
        } else if (itemType == "ConnectorItem") {
            item = new ConnectorItem();
            m_network->addConnector(dynamic_cast<ConnectorItem*>(item));
        }
        if (item) {
            item->setPos(event->scenePos());
            addItemToScene(item);
            event->setAccepted(true);
        } else {
            event->setAccepted(false);
        }
    } else {
        event->setAccepted(false);
    }
}
