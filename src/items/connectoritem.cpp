#include "connectorItem.h"
#include <QPainter>
#include <cmath>

ConnectorItem::ConnectorItem(QGraphicsItem* parent)
    : BaseItem(parent) {
        m_type = ItemType::Connector;
        m_closeProxy->setPos(boundingRect().topRight() + QPointF(5, -25));
        hideButtons();
    }

void ConnectorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(Qt::transparent);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(boundingRect());

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(m_color);
    painter->drawEllipse(QRectF(-m_width / 2, -m_height / 2, m_width, m_height));

    if (isSelected() || !m_hoveredPoint.isNull()) {
        painter->setBrush(Qt::darkGray);
        for (const QPointF& point : connectionPoints()) {
            if (isSelected() || point == m_hoveredPoint) {
                painter->drawEllipse(point, DotRadius, DotRadius);
            }
        }
    }
    if(isSelected()){
        showButtons();
    } else {
        hideButtons();
    }
}


