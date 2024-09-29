#pragma once

#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class VerticalIndicator : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ value)
    Q_PROPERTY(qreal maxValue READ maxValue WRITE setMaxValue)

public:
    explicit VerticalIndicator(QWidget *parent = nullptr);

    qreal value() const;
    qreal maxValue() const;

public slots:
    void incrementValue();
    void onMaxValueChanged(qreal maxValue);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setMaxValue(qreal maxValue);

    qreal m_value;
    qreal m_maxValue;
    QColor m_startColor;
    QColor m_midColor;
    QColor m_endColor;
};