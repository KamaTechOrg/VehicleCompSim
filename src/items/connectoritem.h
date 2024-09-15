#pragma once

#include "baseitem.h"

class ConnectorItem : public BaseItem {
public:
    ConnectorItem(SerializableItem* item = nullptr, QGraphicsItem* parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};
