#include "baseitem.h"
#include <qrandom.h>
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

    // for test only
    // names.emplace_back("name");
    // names.emplace_back("family");
    // names.emplace_back("msg");
    // end test
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
void BaseItem::update_db_data(QList<QVariant> &new_data){
    Db_data.clear();
    Db_data = new_data;
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
//    QString tooltipHtml = "<table border='1' cellspacing='0' cellpadding='3' style='border-collapse: collapse;'><tr>";
//    for (const QString& name : names) {
//        tooltipHtml += QString("<th>%1</th>").arg(name);
//    }
//    tooltipHtml += "</tr><tr>";
//    for (int i = 0; i < Db_data.size(); ++i) {
//        QVariant value = Db_data.value(i);
//        tooltipHtml += QString("<td>%1</td>").arg(value.toString());
//    }
//    tooltipHtml += "</tr></table>";
//    if (!m_persistentTooltip) {
//        m_persistentTooltip = new PersistentTooltip();
//    }
//    m_persistentTooltip->setText(tooltipHtml);
//    m_persistentTooltip->move(event->screenPos());
//    m_persistentTooltip->show();
    QGraphicsItem::hoverEnterEvent(event);
}
void BaseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
//    if (m_persistentTooltip) {
//        m_persistentTooltip->hide();
//    }
    QGraphicsItem::hoverLeaveEvent(event);
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
        qInfo() << "remove";
//         Remove all connected edges
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
        qInfo() << "after clear";


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
