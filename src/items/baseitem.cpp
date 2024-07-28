#include "baseitem.h"
#include <qrandom.h>
#include <QGraphicsSceneHoverEvent>

BaseItem::BaseItem(NodeType type,  QGraphicsItem* parent)
    : QGraphicsItem(parent), m_type(type){
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    m_color = QColor::fromHsv(QRandomGenerator::global()->bounded(360), 64, 192);
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
            QPainterPath path = edge->path();
            if (edge->source() == this) {
                QPointF srcPos = connectionPoint(edge->dest()->pos());
                QPointF destPos = edge->dest()->connectionPoint(edge->source()->pos());
                path.setElementPositionAt(0, srcPos.x(), srcPos.y());
                path.setElementPositionAt(path.elementCount() - 1, destPos.x(), destPos.y());
            } else {
                QPointF destPos = connectionPoint(edge->source()->pos());
                QPointF srcPos = edge->source()->connectionPoint(edge->dest()->pos());
                path.setElementPositionAt(path.elementCount() - 1, destPos.x(), destPos.y());
                path.setElementPositionAt(0, srcPos.x(), srcPos.y());
            }
            edge->setPath(path);
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void BaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseMoveEvent(event);
    update();
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

void BaseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QPointF nearestPoint;
    if (isNearConnectionPoint(event->pos(), &nearestPoint)) {
        m_hoveredPoint = nearestPoint;
        update(); // Trigger a repaint
    }
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
    QGraphicsItem::hoverLeaveEvent(event);
}