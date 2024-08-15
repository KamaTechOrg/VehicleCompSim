#pragma once

#include <QWidget>
#include <QMimeData>
#include <QDrag>

class CustomWidget : public QWidget {
    Q_OBJECT

public:
    CustomWidget(const QString& type, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QString m_type;
    QPoint m_dragStartPosition;
};
