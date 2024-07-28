#pragma once

#include <QGraphicsItem>
#include <QString>
#include <QList>
#include "EdgeItem.h"

class EdgeItem;

class BaseItem : public virtual QGraphicsItem {
public:
    enum class NodeType { Sensor, Connector };

    BaseItem(NodeType type, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    NodeType nodeType() const { return m_type; }

    virtual void addEdge(EdgeItem* edge);
    virtual void removeEdge(EdgeItem* edge);
    QList<EdgeItem*> edges() const { return m_edges; }

    QPointF connectionPoint(const QPointF& otherPoint) const;

    QList<QPointF> connectionPoints() const;
    bool isNearConnectionPoint(const QPointF& point) const;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    bool isNearConnectionPoint(const QPointF& point, QPointF* nearestPoint) const; // Overload for hover events

    NodeType m_type;
    QColor m_color;
    qreal m_width = 25;
    qreal m_height = 25;
    QPointF m_hoveredPoint;
    QList<EdgeItem*> m_edges;

    static constexpr qreal DotRadius = 5.0;
};
