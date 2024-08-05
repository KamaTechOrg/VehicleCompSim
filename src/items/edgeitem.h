#pragma once

#include <QPainter>
#include <QGraphicsLineItem>
#include "BaseItem.h"

class BaseItem;

class EdgeItem : public SerializableItem, public QGraphicsPathItem
{
public:
    EdgeItem(QGraphicsItem* parent = nullptr);

    void addConnection(BaseItem* item);
    // void updatePosition();
    BaseItem* source() const { return m_source; }
    BaseItem* dest() const { return m_dest; }
    void setSource(BaseItem* source);
    void setDest(BaseItem* dest);

    QJsonObject serialize() const override;
    void deserialize(const QJsonObject &itemData) override;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    BaseItem* m_source = nullptr;
    BaseItem* m_dest = nullptr;
};
