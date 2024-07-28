#include "CustomScene.h"
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>
#include <QUuid>
#include <queue>
#include <unordered_set>
#include <cmath>
#include "items/SensorItem.h"
#include "items/ConnectorItem.h"
#include "popupdialog.h"

CustomScene::CustomScene(QObject* parent)
    : QGraphicsScene(parent), m_network(new Network<SensorItem, ConnectorItem>()), m_currentEdge(nullptr), m_startItem(nullptr) {
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
    m_currentEdge = new EdgeItem(QUuid::createUuid().toString());
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

// Helper function to check if a point is inside any BaseItem
bool CustomScene::isPointInsideAnyItem(const QPointF& point) {
    for (QGraphicsItem* item : items()) {
        BaseItem* baseItem = dynamic_cast<BaseItem*>(item);
        if (baseItem && baseItem->contains(baseItem->mapFromScene(point))) {
            return true;
        }
    }
    return false;
}

// Helper function to find nearest Connector
ConnectorItem* CustomScene::findNearestConnector(const QPointF& point) {
    ConnectorItem* nearestConnector = nullptr;
    qreal minDistance = std::numeric_limits<qreal>::max();

    for (QGraphicsItem* item : items()) {
        ConnectorItem* connector = dynamic_cast<ConnectorItem*>(item);
        if (connector) {
            qreal distance = QLineF(point, connector->scenePos()).length();
            if (distance < minDistance) {
                minDistance = distance;
                nearestConnector = connector;
            }
        }
    }

    return nearestConnector;
}

// Helper function to check if a point is valid
bool CustomScene::isPointValid(const QPointF& point) {
    qreal bufferZone = 10.0; // Buffer zone around items to avoid paths touching them
    for (QGraphicsItem* item : items()) {
        BaseItem* baseItem = dynamic_cast<BaseItem*>(item);
        if (baseItem) {
            QRectF inflatedRect = baseItem->boundingRect().adjusted(-bufferZone, -bufferZone, bufferZone, bufferZone);
            if (baseItem->mapRectToScene(inflatedRect).contains(point)) {
                return false;
            }
        }
    }
    return true;
}

// Generate a valid start or end point outside the BaseItem area
QPointF CustomScene::generateValidPointOutsideItem(BaseItem* item, const QPointF& originalPoint) {
    QPointF validPoint = originalPoint;
    while (!isPointValid(validPoint)) {
        QLineF line(item->pos(), validPoint);
        validPoint = line.pointAt(1.1); // Move the point 10% further from the item center
    }
    return validPoint;
}

// A* pathfinding algorithm
QVector<QPointF> CustomScene::findPath(const QPointF& start, const QPointF& end) {
    QPointF startPoint = generateValidPointOutsideItem(dynamic_cast<BaseItem*>(itemAt(start, QTransform())), start);
    QPointF endPoint = generateValidPointOutsideItem(dynamic_cast<BaseItem*>(itemAt(end, QTransform())), end);

    std::priority_queue<AStarNode*, std::vector<AStarNode*>, CompareNode> openSet;
    std::unordered_set<QPointF> closedSet;

    AStarNode* startNode = new AStarNode(startPoint, 0, QLineF(startPoint, endPoint).length(), nullptr);
    openSet.push(startNode);

    while (!openSet.empty()) {
        AStarNode* current = openSet.top();
        openSet.pop();

        if (QLineF(current->point, endPoint).length() < 5) {  // Close enough to end
            QVector<QPointF> path;
            while (current) {
                path.prepend(current->point);
                current = current->parent;
            }
            path.prepend(start); // Add the original start point
            path.append(end); // Add the original end point
            return path;
        }

        closedSet.insert(current->point);

        // Generate neighbors
        for (int dx = -5; dx <= 5; dx += 5) {
            for (int dy = -5; dy <= 5; dy += 5) {
                if (dx == 0 && dy == 0) continue;

                QPointF neighborPoint(current->point.x() + dx, current->point.y() + dy);
                
                if (closedSet.find(neighborPoint) != closedSet.end() || !isPointValid(neighborPoint)) {
                    continue;
                }

                double tentativeGScore = current->gScore + std::sqrt(dx*dx + dy*dy);
                double hScore = QLineF(neighborPoint, endPoint).length();
                
                AStarNode* neighbor = new AStarNode(neighborPoint, tentativeGScore, tentativeGScore + hScore, current);
                openSet.push(neighbor);
            }
        }
    }

    return QVector<QPointF>();  // No path found
}

// Smart edge routing function
void CustomScene::routeEdge(EdgeItem* edge) {
    BaseItem* sourceItem = edge->source();
    BaseItem* destItem = edge->dest();

    if (!sourceItem || !destItem) return;

    QPointF startPoint = sourceItem->connectionPoint(destItem->scenePos());
    QPointF endPoint = destItem->connectionPoint(sourceItem->scenePos());

    QVector<QPointF> path = findPath(startPoint, endPoint);
    
    // Create a smooth path
    QPainterPath smoothPath;
    smoothPath.moveTo(path[0]);
    for (int i = 1; i < path.size() - 1; i++) {
        QPointF p1 = path[i-1];
        QPointF p2 = path[i];
        QPointF p3 = path[i+1];
        smoothPath.cubicTo(p1, p2, p3);
    }
    smoothPath.lineTo(path.last());

    edge->setPath(smoothPath);
    
    // // If dest is a Sensor, find nearest Connector
    // if (dynamic_cast<SensorItem*>(destItem)) {
    //     ConnectorItem* connector = findNearestConnector(endPoint);
    //     if (connector) {
    //         QPointF connectorPoint = connector->connectionPoint(endPoint);
    //         QVector<QPointF> path1 = findPath(startPoint, connectorPoint);
    //         QVector<QPointF> path2 = findPath(connectorPoint, endPoint);
    //         path1.append(path2);
            
    //         // Create a smooth path
    //         QPainterPath smoothPath;
    //         smoothPath.moveTo(path1[0]);
    //         for (int i = 1; i < path1.size() - 1; i++) {
    //             QPointF p1 = path1[i-1];
    //             QPointF p2 = path1[i];
    //             QPointF p3 = path1[i+1];
    //             smoothPath.cubicTo(p1, p2, p3);
    //         }
    //         smoothPath.lineTo(path1.last());

    //         edge->setPath(smoothPath);
    //     }
    // } else {
    //     QVector<QPointF> path = findPath(startPoint, endPoint);
        
    //     // Create a smooth path
    //     QPainterPath smoothPath;
    //     smoothPath.moveTo(path[0]);
    //     for (int i = 1; i < path.size() - 1; i++) {
    //         QPointF p1 = path[i-1];
    //         QPointF p2 = path[i];
    //         QPointF p3 = path[i+1];
    //         smoothPath.cubicTo(p1, p2, p3);
    //     }
    //     smoothPath.lineTo(path.last());

    //     edge->setPath(smoothPath);
    // }
}

void CustomScene::buildConnection(BaseItem *src, BaseItem *dest)
{
    if(src->nodeType() == BaseItem::NodeType::Connector){
        if(dest->nodeType() == BaseItem::NodeType::Connector){
            removeItem(m_currentEdge);
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
    if(dest->nodeType() == BaseItem::NodeType::Connector){
        QPointF midPoint = (startPoint + endPoint) / 2;
        QPointF controlPoint1 = midPoint + QPointF(-50, -50); 
        QPointF controlPoint2 = midPoint + QPointF(50, 50); 

        path.moveTo(startPoint);
        path.cubicTo(controlPoint1, controlPoint2, endPoint);

        EdgeItem* edge = new EdgeItem(QUuid::createUuid().toString());
        edge->setPath(path);
        edge->setSource(src);
        edge->setDest(dest);

        addItem(edge);
        src->addEdge(edge);
        dest->addEdge(edge);

        m_network->connect(dynamic_cast<SensorItem*>(src), dynamic_cast<ConnectorItem*>(dest));
    } else if(dest->nodeType() == BaseItem::NodeType::Sensor){
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

        EdgeItem* edge1 = new EdgeItem(QUuid::createUuid().toString());
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

        EdgeItem* edge2 = new EdgeItem(QUuid::createUuid().toString());
        edge2->setPath(path);
        edge2->setSource(connector);
        edge2->setDest(dest);
        connector->addEdge(edge2);
        dest->addEdge(edge2);

        addItem(connector);
        addItem(edge1);
        addItem(edge2);

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
        QString id = QUuid::createUuid().toString();
        BaseItem* item = nullptr;
        if (itemType == "SensorItem") {
            SensorItem* sensorItem = new SensorItem();
            PopupDialog popup(sensorItem);
            popup.exec();
            if(popup.result() == QDialog::Accepted && sensorItem->isInitialized()){
                item = sensorItem;
                m_network->addElement(dynamic_cast<SensorItem*>(item));
            } else{
                delete item;
                return;
            }
            m_network->addElement(dynamic_cast<SensorItem*>(item));
        } else if (itemType == "ConnectorItem") {
            item = new ConnectorItem();
            m_network->addConnector(dynamic_cast<ConnectorItem*>(item));
        }
        if (item) {
            item->setPos(event->scenePos());
            addItem(item);
            event->setAccepted(true);
        } else {
            event->setAccepted(false);
        }
    } else {
        event->setAccepted(false);
    }
}
