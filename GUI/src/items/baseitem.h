#pragma once

#include <QString>
#include <QList>
#include <QGraphicsProxyWidget>
#include "serializableitem.h"
#include "edgeitem.h"
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QWidget>
#include "serializableitem.h"
#include "state/globalstate.h"

class EdgeItem;


class BaseItem : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    BaseItem(SerializableItem* item = nullptr, QGraphicsItem* parent = nullptr);
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
    void set_m_color(int R, int G, int B){ this->m_color.setRed(R), this->m_color.setGreen(G),this->m_color.setBlue(B); }
    ItemType itemType() const { return m_model->itemType(); }
    SerializableItem* model() const { return m_model; }

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    bool isNearConnectionPoint(const QPointF& point, QPointF* nearestPoint) const;
    void showButtons();
    void hideButtons();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    SerializableItem* m_model;
    QColor m_color;
    qreal m_width = 25;
    qreal m_height = 25;
    QPointF m_hoveredPoint;
    QList<EdgeItem*> m_edges;
    bool m_positionChanged = false;
    QGraphicsProxyWidget* m_closeProxy;
    static constexpr qreal DotRadius = 5.0;

};

