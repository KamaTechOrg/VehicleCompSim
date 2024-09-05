#pragma once

#include <QGraphicsScene>
#include <memory>
#include <QToolBar>
#include "VehicleCompSim/utils/network.hpp"
#include "items/sensoritem.h"
#include "items/connectoritem.h"
#include "items/edgeitem.h"
#include "items/serializableitem.h"
#include "popupdialog.h"


class CustomScene : public QGraphicsScene {
    Q_OBJECT
public:
    CustomScene(QObject* parent = nullptr);
    void modifyItem(QGraphicsItem *item);
    QToolBar* rightToolBar;  // ToolBar for draggable items
    PopupDialog* popupDialog;



private:
    void addItemToScene(SerializableItem* item);
    void removeItemFromScene(SerializableItem* item);
    
    //drag and drop handlers
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    //sensoers's connection handlers    
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;

    void processBaseItem(QGraphicsSceneMouseEvent* event, BaseItem* baseItem);
    void handleEdgeConnection(QGraphicsSceneMouseEvent* event, BaseItem* baseItem);
    void startNewEdge(QGraphicsSceneMouseEvent* event, BaseItem* baseItem);
    void cleanupCurrentEdge();

    void buildConnection(BaseItem* src, BaseItem* dest);


    Network<SensorItem, ConnectorItem>* m_network;
    EdgeItem* m_currentEdge = nullptr;
    BaseItem* m_startItem = nullptr;
    QPointF m_startPoint;




};
