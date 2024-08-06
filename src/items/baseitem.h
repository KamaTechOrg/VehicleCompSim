#pragma once

#include <QGraphicsItem>
#include <QString>
#include <QList>
#include <QGraphicsProxyWidget>

#include "SerializableItem.h"
#include "edgeItem.h"

class EdgeItem;

class BaseItem : public QObject, public SerializableItem, public QGraphicsItem  {
    Q_OBJECT
    public:

    BaseItem(QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    virtual void addEdge(EdgeItem* edge);
    virtual void removeEdge(EdgeItem* edge);
    QList<EdgeItem*> edges() const { return m_edges; }

    QPointF connectionPoint(const QPointF& otherPoint) const;

    QList<QPointF> connectionPoints() const;
    bool isNearConnectionPoint(const QPointF& point) const;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void removeItem();
    void confirmRemove();

    QJsonObject serialize() const override;
    void deserialize(const QJsonObject &itemData) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    bool isNearConnectionPoint(const QPointF& point, QPointF* nearestPoint) const; // Overload for hover events

    void showButtons();
    void hideButtons();

    QColor m_color;
    qreal m_width = 25;
    qreal m_height = 25;
    QPointF m_hoveredPoint;
    QList<EdgeItem*> m_edges;

    QGraphicsProxyWidget* m_closeProxy;

    static constexpr qreal DotRadius = 5.0;
};
