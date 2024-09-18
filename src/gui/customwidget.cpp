#include "customwidget.h"
#include <QPainter>
//#include <QMouseEvent>
#include <QApplication>

CustomWidget::CustomWidget(const WIDGET_TYPES type, QWidget* parent)
    : QWidget(parent), m_type(type) {
    setFixedSize(50, 50);
    setAcceptDrops(true);
}

void CustomWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    switch (m_type) {
    case REGULAR_SENSOR_ITEM:
        painter.drawRoundedRect(QRectF(5, 5, 40, 22.5), 4, 4);
        break;
    case QEMU_SENSOR_ITEM:
        painter.drawPolygon(QPolygonF({QPointF(25, 5), QPointF(5, 25), QPointF(45, 25)}));
        break;
    case BUS_ITEM:
        painter.drawEllipse(12.5, 12.5, 25, 25);
        break;
    default:
        break;
    }
}

void CustomWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
    }
}

void CustomWidget::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    mimeData->setText(QString::number(m_type));
    drag->setMimeData(mimeData);

    QPixmap pixmap(size());
    pixmap.fill(Qt::transparent);
    render(&pixmap);
    drag->setPixmap(pixmap);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}
