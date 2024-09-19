#pragma once

#include <QWidget>
#include <QMimeData>
#include <QDrag>
#include <QPaintEvent>
#include <QMouseEvent>

class CustomWidget : public QWidget {
    Q_OBJECT

public:
    enum WIDGET_TYPES {
        REGULAR_SENSOR_ITEM,
        QEMU_SENSOR_ITEM,
        BUS_ITEM,
        WIDGET_TYPES_COUNT
    };
    CustomWidget(const WIDGET_TYPES type, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    WIDGET_TYPES m_type;
    QPoint m_dragStartPosition;
};
