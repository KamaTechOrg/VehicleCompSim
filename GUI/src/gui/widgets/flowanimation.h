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
    FlowAnimation(QGraphicsScene *scene, std::function<void()> onFinishedCallback, QObject *parent = nullptr);

    void setPoints(const QPointF &src, const QPointF &dest);
    void startAnimation();
    void cancelAnimation(ProjectModel* project);
    bool isRunning() const;

private:
    QGraphicsScene *scene;
    QGraphicsLineItem *lineItem;
    CustomEllipseItem *lightItem;
    QSequentialAnimationGroup *animationGroup;
    QPointF sourcePt, destPt;
    bool isDeleting;
    std::function<void()> onFinishedCallback;

    void setupItems();
    void setupAnimations();
    void updateItems();

private slots:
    void onAnimationFinished();
};