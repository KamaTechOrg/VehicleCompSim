#include "customscene.h"
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include "qemusensormodel.h"
#include "popupdialog.h"
#include "client/websocketclient.h"
#include "customwidget.h"
#include "globalconstants.h"
#include "serializableitem.h"
#include <cstdlib>
#include <ctime>

using namespace globalConstants;

CustomScene::CustomScene(QObject* parent)
    : QGraphicsScene(parent), m_network(new Network<SensorItem, ConnectorItem>()),
    m_globalState(GlobalState::getInstance()) {
    connect(&m_globalState, &GlobalState::currentProjectChanged, this, &CustomScene::onCurrentProjectChanged);
    connect(&m_globalState, &GlobalState::parsedData, this, &CustomScene::onParsedData);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CustomScene::applyRandomFlowAnimation);
    timer->start(10);

    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void CustomScene::addItemToScene(BaseItem *item) 
{
    if(item == nullptr){
        return;
    }
    if (m_currentProject) {
        m_currentProject->addModel(item->model());
        item->model()->notifyItemAdded();
    }
}

void CustomScene::removeItemFromScene(BaseItem *item)
{
    if(item == nullptr){
        return;
    }
    if (m_currentProject) {
        m_currentProject->removeModel(item->model());
        item->model()->notifyItemDeleted();
    }
}

void CustomScene::clearScene() {
    for(auto item: items()){
        removeItem(item);
    }
    m_sensors.clear();
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
            removeItem(m_currentEdge); // TODO : notify to server
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

        addItem(edge); // TODO : notify to server
        src->addEdge(edge);
        dest->addEdge(edge);

        // m_network->connect(dynamic_cast<SensorItem*>(src), dynamic_cast<ConnectorItem*>(dest));
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

        addItem(connector); // TODO : notify to server
        addItem(edge1); // TODO : notify to server
        addItem(edge2); // TODO : notify to server

        // m_network->connect(dynamic_cast<SensorItem*>(src), connector);
        // m_network->connect(dynamic_cast<SensorItem*>(dest), connector);
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
        CustomWidget::WIDGET_TYPES itemType = (CustomWidget::WIDGET_TYPES)(event->mimeData()->text().toInt());

        SerializableItem* item; // = new SerializableItem();

        if (itemType == CustomWidget::REGULAR_SENSOR_ITEM || itemType == CustomWidget::QEMU_SENSOR_ITEM) {
            SensorModel* sensorModel = (itemType == CustomWidget::REGULAR_SENSOR_ITEM ? new SensorModel() : new QemuSensorModel());

            sensorModel->setOwnerID(m_globalState.myClientId());
            sensorModel->setX(event->scenePos().x());
            sensorModel->setY(event->scenePos().y());
            m_globalState.currentProject()->addModel(sensorModel);
            m_globalState.setCurrentSensorModel(sensorModel);
            sensorModel->notifyItemAdded();
        }
        else if (itemType == CustomWidget::BUS_ITEM) {
            // m_network->addConnector(dynamic_cast<ConnectorItem*>(item));
            ConnectorItem * connectorItem = new ConnectorItem();
            connectorItem->setPos(event->scenePos());
            addItemToScene(connectorItem);
        }
        else {
            event->setAccepted(false);
        }
    } else {
        event->setAccepted(false);
    }
}

void CustomScene::onCurrentProjectChanged(ProjectModel* project) {
    clearScene();
    handleProjectConnections(project);

    m_currentProject = project;
    if (m_currentProject) {
        for (SerializableItem* model : m_currentProject->models()) {
            QGraphicsItem* item = buildBaseItemFromModel(model);
            if (item) {
                addItem(item);
                if(model->itemType() == ItemType::Sensor){
                    SensorItem* sensorItem = dynamic_cast<SensorItem*>(item);
                    m_sensors[sensorItem->getModel().priority()] = sensorItem;
                }
            }
        }
    }
}
QString CustomScene::dataToHtml(const QList<QPair<QString, QString>>& data){
    QString tableHtml = "<table border='1' cellspacing='0' cellpadding='5' style='border-collapse: collapse;'>";
    tableHtml += "<tr style='background-color: #f2f2f2;'>";
    for (const auto& pair : data) {
        tableHtml += QString("<th style='padding: 10px;'>%1</th>").arg(pair.first.toHtmlEscaped());
    }
    tableHtml += "</tr>";
    tableHtml += "<tr>";
    for (const auto& pair : data) {
        tableHtml += QString("<td style='padding: 10px;'>%1</td>").arg(pair.second.toHtmlEscaped());
    }
    tableHtml += "</tr>";
    tableHtml += "</table>";
    return tableHtml;
}

void CustomScene::onParsedData(QList<QPair<QString, QString>> data) {
    SensorItem* src = m_sensors[data[BufferInfo::SourceId].second];
    SensorItem* dest = m_sensors[data[BufferInfo::DestinationId].second];
    QString newData = dataToHtml(data);
    QString srcName = data[SourceId].second;
    QString destName = data[DestinationId].second;
    GlobalState::getInstance().log(newData,  srcName);
    GlobalState::getInstance().log(newData,  destName);
    if(src){
        src->update_new_data(newData);
    }
    if(dest){
        dest->update_new_data(newData);
    }
    if (!src || !dest) {
        qWarning() << "Source or Destination sensor not found.";
        return;
    }

    // Create and start the FlowAnimation
    FlowAnimation* flowAnimation = new FlowAnimation(this, [dest]() {
        dest->getVerticalIndicator()->incrementValue();
    });
    flowAnimation->setPoints(src->pos(), dest->pos());
    flowAnimation->startAnimation();
}

void CustomScene::applyRandomFlowAnimation() {
    if(!m_globalState.isTest()){
        return;
    }
    if (m_sensors.size() < 2) {
        return;
    }

    // Select two random SensorItems
    int index1 = std::rand() % m_sensors.size();
    int index2;
    do {
        index2 = std::rand() % m_sensors.size();
    } while (index1 == index2);

    SensorItem* src = m_sensors.values()[index1];
    SensorItem* dest = m_sensors.values()[index2];

    if (!src || !dest) {
        qWarning() << "Source or Destination sensor not found.";
        return;
    }

    // Create and start the FlowAnimation
    FlowAnimation* flowAnimation = new FlowAnimation(this, [dest]() {
        dest->getVerticalIndicator()->incrementValue();
    });
    flowAnimation->setPoints(src->pos(), dest->pos());
    flowAnimation->startAnimation();
}


void CustomScene::handleProjectConnections(ProjectModel* newProject) {
    if (m_currentProject) {
        disconnect(m_currentProject, &ProjectModel::modelAdded, this, &CustomScene::onModelAdded);
        disconnect(m_currentProject, &ProjectModel::modelRemoved, this, &CustomScene::onModelRemoved);
        disconnect(m_currentProject, &ProjectModel::modelUpdated, this, &CustomScene::onModelUpdated);
    }

    connect(newProject, &ProjectModel::modelAdded, this, &CustomScene::onModelAdded);
    connect(newProject, &ProjectModel::modelRemoved, this, &CustomScene::onModelRemoved);
    connect(newProject, &ProjectModel::modelUpdated, this, &CustomScene::onModelUpdated);
}

void CustomScene::onModelAdded(SerializableItem* model) {
    BaseItem* item = buildBaseItemFromModel(model);
    if (item) {
        addItem(item);
        if(model->itemType() == ItemType::Sensor){
            SensorItem* sensorItem = dynamic_cast<SensorItem*>(item);
            m_sensors[sensorItem->getModel().priority()] = sensorItem;
        }
    }
}

void CustomScene::onModelRemoved(SerializableItem* model) {
    for (QGraphicsItem* item : items()) {
        SensorItem* sensorItem = dynamic_cast<SensorItem*>(item);
        if (sensorItem && sensorItem->model()->getId() == model->getId()) {
            m_sensors.remove(sensorItem->getModel().priority());
            removeItem(sensorItem);
            sensorItem->deleteLater();
            break;
        }
    }
}

void CustomScene::onModelUpdated(SerializableItem* model) {
    // for (QGraphicsItem* item : items()) {
    //     BaseItem* baseItem = dynamic_cast<BaseItem*>(item);
    //     if (baseItem && baseItem->model()->getId() == model->getId()) {
    //         // modifyItem(item);
    //         break;
    //     }
    // }
    // TODO
}

BaseItem* CustomScene::buildBaseItemFromModel(SerializableItem* model) {
    if (model->itemType() == ItemType::Sensor) {
        SensorModel* sensorModel = dynamic_cast<SensorModel*>(model);
        return new SensorItem(sensorModel);
    } else if (model->itemType() == ItemType::Connector) {
        return new ConnectorItem();
    }
    // TODO : add edge
    return nullptr;
}
