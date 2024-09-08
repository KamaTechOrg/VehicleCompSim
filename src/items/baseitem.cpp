#include "baseitem.h"
#include <qrandom.h>
//#include <QIcon>
#include <QGraphicsSceneHoverEvent>
#include <QPushButton>
#include <QMessageBox>
#include <QGraphicsScene>
qreal BaseItem::my_id = 0;

BaseItem::BaseItem(QGraphicsItem* parent) : SerializableItem(), QGraphicsItem(parent) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    
    m_color = QColor::fromHsv(QRandomGenerator::global()->bounded(360), 64, 192);

    // Create close button
    QPushButton* closeButton = new QPushButton("X");
    //closeButton->setIcon(QIcon(":/icons/delete.png"));
    closeButton->setFixedSize(20, 20);
    closeButton->setToolTip("Remove Item");

    QGraphicsProxyWidget* closeProxy = new QGraphicsProxyWidget(this);
    closeProxy->setWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &BaseItem::confirmRemove);

    // Store proxies for visibility management
    m_closeProxy = closeProxy;

    unique_id = my_id;
    my_id++;
}

QRectF BaseItem::boundingRect() const {
    return QRectF(-m_width / 2 - DotRadius, -m_height / 2 - DotRadius, m_width + 2*DotRadius, m_height + 2*DotRadius);
}
void BaseItem::addEdge(EdgeItem* edge) {
    if (!m_edges.contains(edge)) {
        m_edges.append(edge);
    }
}

void BaseItem::removeEdge(EdgeItem* edge) {
    m_edges.removeAll(edge);
}

QVariant BaseItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged) {
        for (EdgeItem* edge : m_edges) {
            QPointF srcPos, destPos;
            if (edge->source() == this) {
                srcPos = connectionPoint(edge->dest()->pos());
                destPos = edge->dest()->connectionPoint(edge->source()->pos());
            } else {
                destPos = connectionPoint(edge->source()->pos());
                srcPos = edge->source()->connectionPoint(edge->dest()->pos());
            }

            // smooth the edge
            QPointF midPoint = (srcPos + destPos) / 2;
            QPointF controlPoint1 = midPoint + QPointF(-50, -50);
            QPointF controlPoint2 = midPoint + QPointF(50, 50);

            QPainterPath path;
            path.moveTo(srcPos);
            path.cubicTo(controlPoint1, controlPoint2, destPos);

            edge->setPath(path);
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void BaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseMoveEvent(event);
    update();
}
void BaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    notifyItemModified();
}

void BaseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QPointF nearestPoint;
    if (isNearConnectionPoint(event->pos(), &nearestPoint)) {
        m_hoveredPoint = nearestPoint;
        update(); // Trigger a repaint
    }

    // Create a string with the private fields' information
    QString tooltipText = QString("Color: %1\nHovered Point: (%2, %3)")
            .arg(m_color.name())
            .arg(m_hoveredPoint.x())
            .arg(m_hoveredPoint.y());

    QToolTip::showText(event->screenPos(), tooltipText, nullptr);
    QGraphicsItem::hoverEnterEvent(event);

}

void BaseItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    QPointF nearestPoint;
    if (isNearConnectionPoint(event->pos(), &nearestPoint)) {
        if (nearestPoint != m_hoveredPoint) {
            m_hoveredPoint = nearestPoint;
            update(); // Trigger a repaint
        }
    } else {
        if (!m_hoveredPoint.isNull()) {
            m_hoveredPoint = QPointF();
            update(); // Trigger a repaint
        }
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void BaseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    m_hoveredPoint = QPointF();
    update(); // Trigger a repaint
    QToolTip::hideText();
    QGraphicsItem::hoverLeaveEvent(event);
}

void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(m_color);
    painter->drawRect(boundingRect());
}

QPointF BaseItem::connectionPoint(const QPointF& otherPoint) const {
    QPointF sceneOtherPoint = mapFromScene(otherPoint);
    QPointF closestPoint;
    qreal minDist = std::numeric_limits<qreal>::max();
    for (const QPointF& point : connectionPoints()) {
        qreal dist = QLineF(sceneOtherPoint, point).length();
        if (dist < minDist) {
            minDist = dist;
            closestPoint = point;
        }
    }
    return mapToScene(closestPoint);
}

QList<QPointF> BaseItem::connectionPoints() const {
    QRectF rect(-m_width / 2, -m_height / 2, m_width, m_height); 
    return {
        QPointF(rect.left() + rect.width() / 2, rect.top()), // Top middle
        QPointF(rect.right(), rect.top() + rect.height() / 2), // Right middle
        QPointF(rect.left() + rect.width() / 2, rect.bottom()), // Bottom middle
        QPointF(rect.left(), rect.top() + rect.height() / 2) // Left middle
    };
}

bool BaseItem::isNearConnectionPoint(const QPointF& point) const {
    for (const QPointF& connectionPoint : connectionPoints()) {
        QPointF sceneConnectionPoint = mapToScene(connectionPoint);
        if (QLineF(point, sceneConnectionPoint).length() <= DotRadius * 2) {
            return true;
        }
    }
    return false;
}

bool BaseItem::isNearConnectionPoint(const QPointF& point, QPointF* nearestPoint) const {
    for (const QPointF& connectionPoint : connectionPoints()) {
        if (QLineF(point, connectionPoint).length() <= DotRadius * 2) {
            *nearestPoint = connectionPoint;
            return true;
        }
    }
    return false;
}

void BaseItem::showButtons()
{
    m_closeProxy->setVisible(true);
}

void BaseItem::hideButtons()
{
    m_closeProxy->setVisible(false);
}


void BaseItem::confirmRemove() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Confirm Removal", 
        "Are you sure you want to remove this item?",
        QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        removeItem();
    }
}

void BaseItem::removeItem() {
    if (scene()) {
        // Remove all connected edges
        for (EdgeItem* edge : m_edges) {
            auto connectedItem = edge->source() == this ? edge->dest() : edge->source();
            if(connectedItem->itemType() == ItemType::Connector){
                if(connectedItem->edges().size() <= 2){
                    connectedItem->removeItem();
                }
                else{
                    connectedItem->removeEdge(edge);
                    scene()->removeItem(edge);
                    edge->notifyItemDeleted();
                    delete edge;
                }
            }
            else{
                scene()->removeItem(edge);
                edge->notifyItemDeleted();
                delete edge;                    
            }
        }
        m_edges.clear();

        // Remove this item
        scene()->removeItem(this);
        notifyItemDeleted();
        deleteLater();
    }
}

QJsonObject BaseItem::serialize() const {
    QJsonObject itemData = SerializableItem::serialize();
    itemData["type"] = static_cast<int>(m_type);
    itemData["x"] = pos().x();
    itemData["y"] = pos().y();
    return itemData;
}

void BaseItem::deserialize(const QJsonObject &itemData) {
    SerializableItem::deserialize(itemData);
    m_type = static_cast<ItemType>(itemData["type"].toInt());
    setPos(itemData["x"].toDouble(), itemData["y"].toDouble());
}



