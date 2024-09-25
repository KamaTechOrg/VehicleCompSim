#pragma once

#include <QGraphicsScene>
#include <memory>
#include <QToolBar>
#include "network.hpp"
#include "sensoritem.h"
#include "connectoritem.h"
#include "edgeitem.h"
#include "serializableitem.h"
#include "projectmodel.h"
#include "globalstate.h"
#include "widgets/flowanimation.h"

class CustomScene : public QGraphicsScene {
    Q_OBJECT
public:
    CustomScene(QObject* parent = nullptr);
    void modifyItem(QGraphicsItem *item);
    
public slots:
    void onCurrentProjectChanged(ProjectModel* project);
    void onModelAdded(SerializableItem* model);
    void onModelRemoved(SerializableItem* model);
    void onModelUpdated(SerializableItem* model);

    void applyRandomFlowAnimation();

    
private:
    QToolBar* rightToolBar;  // ToolBar for draggable items
    PopupDialog* popupDialog;
    void addItemToScene(BaseItem* item);
    void removeItemFromScene(BaseItem* item);
    void clearScene();
    BaseItem* buildBaseItemFromModel(SerializableItem* model);

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

    void handleProjectConnections(ProjectModel* newProject);

    ProjectModel* m_currentProject = nullptr;
    Network<SensorItem, ConnectorItem>* m_network;
    EdgeItem* m_currentEdge = nullptr;
    BaseItem* m_startItem = nullptr;
    QPointF m_startPoint;
    GlobalState &m_globalState;
};
