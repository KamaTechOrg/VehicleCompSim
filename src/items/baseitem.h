#pragma once

//#include <QGraphicsItem>
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
#include "CustomInfoWindow.h"


class EdgeItem;

class BaseItem : public QObject, public SerializableItem, public QGraphicsItem {
Q_OBJECT
public:

    BaseItem(QGraphicsItem* parent = nullptr);

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

    QJsonObject serialize() const override;
    void deserialize(const QJsonObject &itemData) override;
public:
    qreal get_m_width(){ return this->m_width; }
    QColor get_m_color(){ return this->m_color; }
    qreal get_m_height(){ return this->m_height; }
    QPointF get_m_hoveredPoint(){ return this->m_hoveredPoint; }
    qreal get_unique_id(){ return unique_id; }
    void set_m_width(qreal width){ this->m_width = width; }
    void set_m_height(qreal height){ this->m_height = height; }
    void set_m_color(int R, int G, int B){ this->m_color.setRed(R), this->m_color.setGreen(G),this->m_color.setBlue(B); }
    void set_unique_id(qreal id){ this->unique_id = id; }
//    void set_nodeType(NodeType type) { this->m_type = type; }
    void set_m_hoveredPoint(){ this->m_hoveredPoint; }

    void update_db_data(QList<QVariant> &new_data);



protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    bool isNearConnectionPoint(const QPointF& point, QPointF* nearestPoint) const;
    void showButtons();
    void hideButtons();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



    QColor m_color;
    qreal m_width = 25;
    qreal m_height = 25;
    QPointF m_hoveredPoint;
    QList<EdgeItem*> m_edges;
    qreal unique_id;

    QGraphicsProxyWidget* m_closeProxy;

    static constexpr qreal DotRadius = 5.0;
    static qreal my_id;

    PersistentTooltip* m_persistentTooltip = nullptr;
    QList<QVariant> tooltipData;
    QList<QVariant> all_Db_data;

public:
    QList<QString> names;
private:

    void showInfoWindow();
    QString fetchDataFromDB();
    void updateInfoWindow();

    QTimer* m_updateTimer;
    QGraphicsProxyWidget* m_infoWindowProxy = nullptr;
    CustomInfoWindow* m_infoWindow = nullptr;
    bool mouse_pressed = false;
    int m_itemId;

private slots:
    void onCustomWindowClosed() {
        m_updateTimer->stop();
        mouse_pressed = false;
    }
};

