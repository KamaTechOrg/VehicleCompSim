#pragma once

#include <QWidget>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class VerticalIndicator : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(qreal maxValue READ maxValue WRITE setMaxValue NOTIFY maxValueChanged)
    Q_PROPERTY(QColor startColor READ startColor WRITE setStartColor NOTIFY startColorChanged)
    Q_PROPERTY(QColor midColor READ midColor WRITE setMidColor NOTIFY midColorChanged)
    Q_PROPERTY(QColor endColor READ endColor WRITE setEndColor NOTIFY endColorChanged)

public:
    explicit VerticalIndicator(QWidget *parent = nullptr);

    qreal value() const;
    qreal maxValue() const;
    QColor startColor() const;
    QColor midColor() const;
    QColor endColor() const;

public slots:
    void setValue(qreal value);
    void setMaxValue(qreal maxValue);
    void setStartColor(const QColor &color);
    void setMidColor(const QColor &color);
    void setEndColor(const QColor &color);

signals:
    void valueChanged(qreal value);
    void maxValueChanged(qreal maxValue);
    void startColorChanged(const QColor &color);
    void midColorChanged(const QColor &color);
    void endColorChanged(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    qreal m_value;
    qreal m_maxValue;
    QColor m_startColor;
    QColor m_midColor;
    QColor m_endColor;
};