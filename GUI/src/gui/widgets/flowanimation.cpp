#include "flowanimation.h"
#include "CustomEllipseItem.h"

FlowAnimation::FlowAnimation(QGraphicsScene *scene, std::function<void()> onFinishedCallback, QObject *parent)
    : QObject(parent), scene(scene), lineItem(nullptr), lightItem(nullptr), animationGroup(nullptr), isDeleting(false), onFinishedCallback(onFinishedCallback) {
    setupItems();
    setupAnimations();
    connect(&GlobalState::getInstance(), &GlobalState::currentProjectChanged, this, &FlowAnimation::cancelAnimation);
}

void FlowAnimation::setPoints(const QPointF &src, const QPointF &dest) {
    sourcePt = src;
    destPt = dest;
    updateItems();
}

void FlowAnimation::startAnimation() {
    if (animationGroup) {
        animationGroup->start();
    }
}

void FlowAnimation::setupItems() {
    QColor lightColor(0, 255, 255); // Define the light color

    lineItem = scene->addLine(QLineF(), QPen(QColor(28, 130, 166), 2, Qt::SolidLine));
    lineItem->setZValue(-1); // Set z-value to a lower value
    
    // Create light effect
    QRadialGradient gradient(QPointF(0, 0), 10);
    gradient.setColorAt(0, QColor(5, 75, 100));
    gradient.setColorAt(0.5, QColor(0, 255, 255, 150));
    gradient.setColorAt(1, QColor(0, 255, 255, 0));
    
    CustomEllipseItem *ellipseItem = new CustomEllipseItem(QRectF(-5, -5, 10, 10));
    ellipseItem->setBrush(QBrush(gradient));
    ellipseItem->setZValue(-1); // Set z-value to a lower value
    
    // Add improved glow effect
    QGraphicsDropShadowEffect *glowEffect = new QGraphicsDropShadowEffect;
    glowEffect->setColor(lightColor);
    glowEffect->setBlurRadius(20); // Increase blur radius for a more pronounced effect
    glowEffect->setOffset(0);
    ellipseItem->setGraphicsEffect(glowEffect);
    
    scene->addItem(ellipseItem); // Use ellipseItem directly
    lightItem = ellipseItem; // Set it as the animated item
}

void FlowAnimation::setupAnimations() {
    animationGroup = new QSequentialAnimationGroup(this);
    
    QPropertyAnimation *flowAnimation = new QPropertyAnimation(lightItem, "pos");
    flowAnimation->setDuration(500);  // 1 second for moving
    
    animationGroup->addAnimation(flowAnimation);
    
    connect(animationGroup, &QSequentialAnimationGroup::finished, this, &FlowAnimation::onAnimationFinished);
}

void FlowAnimation::updateItems() {
    if (lineItem) {
        lineItem->setLine(QLineF(sourcePt, destPt));
    }
    
    QPropertyAnimation *flowAnimation = qobject_cast<QPropertyAnimation*>(animationGroup->animationAt(0));
    if (flowAnimation) {
        flowAnimation->setStartValue(sourcePt);
        flowAnimation->setEndValue(destPt);
    }
}

void FlowAnimation::onAnimationFinished() {
    if (scene) {
        if (lineItem) {
            scene->removeItem(lineItem);
            delete lineItem;
            lineItem = nullptr;
        }
        if (lightItem) {
            scene->removeItem(lightItem);
            delete lightItem;
            lightItem = nullptr;
        }
    }
    if (onFinishedCallback) {
        onFinishedCallback();
    }
    deleteLater();  // Clean up the FlowAnimation object
}

void FlowAnimation::cancelAnimation(ProjectModel* project) {
    if (isDeleting) {
        return;
    }
    if (animationGroup) {
        if (animationGroup->state() != QAbstractAnimation::Stopped) {
            animationGroup->stop();
        }
        delete animationGroup;
        animationGroup = nullptr;
    }
    if (scene) {
        if (lineItem) {
            scene->removeItem(lineItem);
            delete lineItem;
            lineItem = nullptr;
        }
        if (lightItem) {
            scene->removeItem(lightItem);
            delete lightItem;
            lightItem = nullptr;
        }
    }
    isDeleting = true;
    deleteLater();  // Clean up the FlowAnimation object
}

bool FlowAnimation::isRunning() const {
    return animationGroup && animationGroup->state() == QAbstractAnimation::Running;
}