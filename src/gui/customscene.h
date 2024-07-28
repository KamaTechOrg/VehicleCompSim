#pragma once

#include <QGraphicsScene>
#include <memory>
#include "VehicleCompSim/utils/network.hpp"
#include "items/SensorItem.h"
#include "items/ConnectorItem.h"
#include "items/EdgeItem.h"
#include "items/ConnectorItem.h"

class CustomScene : public QGraphicsScene {
    Q_OBJECT
public:
    CustomScene(QObject* parent = nullptr);

protected:
    //drag and drop handlers
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    //sensoers's connection handlers    
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;

private:
    void processBaseItem(QGraphicsSceneMouseEvent* event, BaseItem* baseItem);
    void handleEdgeConnection(QGraphicsSceneMouseEvent* event, BaseItem* baseItem);
    void startNewEdge(QGraphicsSceneMouseEvent* event, BaseItem* baseItem);
    void cleanupCurrentEdge();
    bool isPointValid(const QPointF& point);
    bool isPointInsideAnyItem(const QPointF& point);
    QPointF generateValidPointOutsideItem(BaseItem* item, const QPointF& originalPoint);
    ConnectorItem* findNearestConnector(const QPointF& point);
    QVector<QPointF> findPath(const QPointF& start, const QPointF& end);
    void routeEdge(EdgeItem* edge);

    void buildConnection(BaseItem* src, BaseItem* dest);

    Network<SensorItem, ConnectorItem>* m_network;
    EdgeItem* m_currentEdge;
    BaseItem* m_startItem;
    QPointF m_startPoint;
};

struct AStarNode {
    QPointF point;
    double gScore;
    double fScore;
    AStarNode* parent;

    AStarNode(QPointF p, double g, double f, AStarNode* par)
        : point(p), gScore(g), fScore(f), parent(par) {}
};

struct CompareNode {
    bool operator()(const AStarNode* a, const AStarNode* b) {
        return a->fScore > b->fScore;
    }
};

namespace std {
    template <>
    struct hash<QPointF> {
        std::size_t operator()(const QPointF& point) const {
            std::size_t h1 = std::hash<double>()(point.x());
            std::size_t h2 = std::hash<double>()(point.y());
            return h1 ^ (h2 << 1);
        }
    };
}