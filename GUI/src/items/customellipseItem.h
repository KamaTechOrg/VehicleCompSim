#pragma once

#include <QGraphicsObject>
#include <QGraphicsEllipseItem>

class CustomEllipseItem : public QGraphicsObject {
    Q_OBJECT

public:
    CustomEllipseItem(const QRectF &rect, QGraphicsItem *parent = nullptr)
        : QGraphicsObject(parent), ellipseItem(new QGraphicsEllipseItem(rect, this)) {
        ellipseItem->setPen(Qt::NoPen);
    }

    QRectF boundingRect() const override {
        return ellipseItem->boundingRect();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        ellipseItem->paint(painter, option, widget);
    }

    void setBrush(const QBrush &brush) {
        ellipseItem->setBrush(brush);
    }

    void setGraphicsEffect(QGraphicsEffect *effect) {
        ellipseItem->setGraphicsEffect(effect);
    }

private:
    QGraphicsEllipseItem *ellipseItem;
};