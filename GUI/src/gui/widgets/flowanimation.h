#pragma once

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QRadialGradient>
#include <QGraphicsDropShadowEffect>
#include "CustomEllipseItem.h"
#include "globalstate.h"

class FlowAnimation : public QObject {
    Q_OBJECT

public:
    FlowAnimation(QGraphicsScene *scene, QObject *parent = nullptr);

    void setPoints(const QPointF &src, const QPointF &dest);
    void startAnimation();
    void cancelAnimation(ProjectModel* project); // New method to cancel the animation
    bool isRunning() const; // New method to check if the animation is running

private:
    QGraphicsScene *scene;
    QGraphicsLineItem *lineItem;
    CustomEllipseItem *lightItem; // Change type to CustomEllipseItem
    QSequentialAnimationGroup *animationGroup;
    QPointF sourcePt, destPt;
    bool isDeleting;

    void setupItems();
    void setupAnimations();
    void updateItems();

private slots:
    void onAnimationFinished();
};