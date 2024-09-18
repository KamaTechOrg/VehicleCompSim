#include "baseitem.h"
#include "customscene.h"
#include <qrandom.h>
#include <QGraphicsSceneHoverEvent>
#include <QPushButton>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <GlobalState.h>

// TODO remove this
qreal BaseItem::my_id = 0;

BaseItem::BaseItem(SerializableItem* item, QGraphicsItem* parent) : QGraphicsItem(parent) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptHoverEvents(true);
    if(item){
        m_model = item;
        setPos(m_model->x(), m_model->y());
    } else {
        m_model = new SerializableItem();
    }
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
    // Set up timer for live updates
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &BaseItem::showInfoWindow);
}

void BaseItem::showInfoWindow() {
    if (!scene()) return;
    m_updateTimer->start(1000);  // Update every second
    if (m_infoWindowProxy == nullptr) {
        m_infoWindow = new CustomInfoWindow();
        m_infoWindowProxy = new QGraphicsProxyWidget(this);
        m_infoWindowProxy = scene()->addWidget(m_infoWindow);
        m_infoWindowProxy->setZValue(1);
        connect(m_infoWindow, &CustomInfoWindow::closed, this, &BaseItem::onCustomWindowClosed);
    }
    updateInfoWindow();
    QPointF pos = mapToScene(boundingRect().topRight() + QPointF(10, 0));
    m_infoWindowProxy->setPos(pos);
    m_infoWindowProxy->show();
//    m_updateTimer->start(1000);  // Update every second

}
void BaseItem::updateInfoWindow() {
    if (!m_infoWindow) return;
    QString info = fetchDataFromDB();
    m_infoWindow->setInfo(info);
}

QString BaseItem::fetchDataFromDB() {
    QString tooltipHtml = "<table border='1' cellspacing='0' cellpadding='3' style='border-collapse: collapse;'>";
    tooltipHtml += "<tr>";
    for (const QString &name: names) {
        tooltipHtml += QString("<th>%1</th>").arg(name);
    }
    tooltipHtml += "</tr>";
    for (int i = 0; i < all_Db_data.size(); i += names.size()) {
        tooltipHtml += "<tr>";
        for (int j = 0; j < names.size() && (i + j) < all_Db_data.size(); ++j) {
            QVariant value = all_Db_data.value(i + j);
            tooltipHtml += QString("<td>%1</td>").arg(value.toString());
        }
        tooltipHtml += "</tr>";
    }

    tooltipHtml += "</table>";
    return tooltipHtml;
}

void BaseItem::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        mouse_pressed = true;
        showInfoWindow();
    }
    QGraphicsItem::mousePressEvent(event);
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
        m_positionChanged = true;

        m_model->setX(pos().x());
        m_model->setY(pos().y());
    }
    return QGraphicsItem::itemChange(change, value);
}

void BaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseMoveEvent(event);
    update();
}
void BaseItem::update_db_data(QList<QVariant> &new_data){
    tooltipData.clear();
    all_Db_data.clear();
    all_Db_data = new_data;
    if(!new_data.empty()) {
        for (int i = 0; i < names.size(); i++) {
            tooltipData.emplace_back(new_data[i]);
        }
    }
}

void BaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    if (m_positionChanged) {
        m_positionChanged = false;
        m_model->notifyItemModified();
    }
}

void BaseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QPointF nearestPoint;
    if (isNearConnectionPoint(event->pos(), &nearestPoint)) {
        m_hoveredPoint = nearestPoint;
        update(); // Trigger a repaint
    }
    if(!mouse_pressed){
        QString tooltipHtml = "<table border='1' cellspacing='0' cellpadding='3' style='border-collapse: collapse;'><tr>";
        for (const QString &name: names) {
            tooltipHtml += QString("<th>%1</th>").arg(name);
        }
        tooltipHtml += "</tr><tr>";
        for (int i = 0; i < tooltipData.size(); ++i) {
            QVariant value = tooltipData.value(i);
            tooltipHtml += QString("<td>%1</td>").arg(value.toString());
        }
        tooltipHtml += "</tr></table>";
        if (m_persistentTooltip == nullptr) {
            m_persistentTooltip = new PersistentTooltip();
        }
        m_persistentTooltip->setText(tooltipHtml);
        m_persistentTooltip->move(event->screenPos());
        m_persistentTooltip->show();
    }
    QGraphicsItem::hoverEnterEvent(event);
}
void BaseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    if (m_persistentTooltip) {
        m_persistentTooltip->hide();
    }
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
//        qInfo() << "remove";
//         Remove all connected edges
        for (EdgeItem* edge : m_edges) {
            auto connectedItem = edge->source() == this ? edge->dest() : edge->source();
            if(connectedItem->m_model->itemType() == ItemType::Connector){
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
//        qInfo() << "after clear";


        // Remove this item
        scene()->removeItem(this);
        m_model->notifyItemDeleted();// TODO - CHECK IF TO DELETE THIS
        GlobalState::getInstance().currentProject()->removeModel(m_model);
        deleteLater();
    }
}
