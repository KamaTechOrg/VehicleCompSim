#include "EdgeItem.h"
#include <QPen>

EdgeItem::EdgeItem(const QString& id, QGraphicsItem* parent)
    : QGraphicsPathItem(parent), m_source(nullptr), m_dest(nullptr), m_id(id) {
    setPen(QPen(Qt::black, 2));
}

void EdgeItem::addConnection(BaseItem* item) {
    if (!m_source) {
        m_source = item;
    } else {
        m_dest = item;
        // updatePosition();
    }
}

// void EdgeItem::updatePosition() {
//     if (m_source && m_dest) {
//         QLineF line(mapFromItem(m_source, m_source->connectionPoint(m_dest->pos())),
//                     mapFromItem(m_dest, m_dest->connectionPoint(m_source->pos())));
//         setLine(line);
//     }
// }

void EdgeItem::setSource(BaseItem* source) {
    m_source = source;
}

void EdgeItem::setDest(BaseItem* dest) {
    m_dest = dest;
}

void EdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsPathItem::paint(painter, option, widget);
}