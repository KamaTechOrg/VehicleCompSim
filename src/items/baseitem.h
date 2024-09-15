#pragma once

#include <QString>
#include <QList>
#include <QGraphicsProxyWidget>
#include "serializableitem.h"
#include "edgeitem.h"
#include "PersistentTootip.h"
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QToolTip>
#include <QWidget>

#include "serializableitem.h"

class EdgeItem;

class BaseItem : public QObject, public QGraphicsItem {
    Q_OBJECT
public:

    BaseItem(SerializableItem* item = nullptr, QGraphicsItem* parent = nullptr);

    ~BaseItem() {
        delete m_persistentTooltip;
    }

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

public:
    QPointF get_m_hoveredPoint(){ return this->m_hoveredPoint; }
    qreal get_unique_id(){ return unique_id; }
    void set_m_width(qreal width){ this->m_width = width; }
    void set_m_height(qreal height){ this->m_height = height; }
    void set_m_color(int R, int G, int B){ this->m_color.setRed(R), this->m_color.setGreen(G),this->m_color.setBlue(B); }
    void set_unique_id(qreal id){ this->unique_id = id; }

    ItemType itemType() const { return m_model->itemType(); }

    SerializableItem* model() const { return m_model; }
  
    void update_db_data(QList<QVariant> &new_data);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    bool isNearConnectionPoint(const QPointF& point, QPointF* nearestPoint) const; // Overload for hover events

    void showButtons();
    void hideButtons();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    SerializableItem* m_model;

    QColor m_color;
    qreal m_width = 25;
    qreal m_height = 25;
    QPointF m_hoveredPoint;
    QList<EdgeItem*> m_edges;
    qreal unique_id;
    bool m_positionChanged = false;

    QGraphicsProxyWidget* m_closeProxy;

    static constexpr qreal DotRadius = 5.0;
    static qreal my_id;

    PersistentTooltip* m_persistentTooltip = nullptr;
    QList<QVariant> Db_data;
public:
    QList<QString> names;
};

