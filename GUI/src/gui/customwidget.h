#pragma once

#include "serializableitem.h"
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
        MAIN_COMPUTER_ITEM,
        BUS_ITEM,
        WIDGET_TYPES_COUNT
    };
    CustomWidget(const WIDGET_TYPES type, QWidget* parent = nullptr);
    static SerializableItem* toSerializableItem(WIDGET_TYPES type);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    WIDGET_TYPES m_type;
    QPoint m_dragStartPosition;
};
