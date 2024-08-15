#pragma once

#include "baseitem.h"

class ConnectorItem : public BaseItem {
public:
    ConnectorItem(QGraphicsItem* parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};
