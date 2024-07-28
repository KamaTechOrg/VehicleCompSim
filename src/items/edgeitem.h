#pragma once

#include <QPainter>
#include <QGraphicsLineItem>
#include "BaseItem.h"

class BaseItem;

class EdgeItem : public QGraphicsPathItem {
public:
    EdgeItem(const QString& id, QGraphicsItem* parent = nullptr);

    void addConnection(BaseItem* item);
    // void updatePosition();
    BaseItem* source() const { return m_source; }
    BaseItem* dest() const { return m_dest; }
    void setSource(BaseItem* source);
    void setDest(BaseItem* dest);

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    BaseItem* m_source;
    BaseItem* m_dest;
    QString m_id;
};
