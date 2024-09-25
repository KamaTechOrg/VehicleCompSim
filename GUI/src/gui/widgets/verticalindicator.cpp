#include "verticalindicator.h"

VerticalIndicator::VerticalIndicator(QWidget *parent)
    : QWidget(parent), m_value(0), m_maxValue(100),
      m_startColor(Qt::green), m_midColor(Qt::yellow), m_endColor(Qt::red)
{
    setFixedWidth(10);
    setFixedHeight(90);
}

qreal VerticalIndicator::value() const { return m_value; }
qreal VerticalIndicator::maxValue() const { return m_maxValue; }
QColor VerticalIndicator::startColor() const { return m_startColor; }
QColor VerticalIndicator::midColor() const { return m_midColor; }
QColor VerticalIndicator::endColor() const { return m_endColor; }

void VerticalIndicator::setValue(qreal value)
{
    if (value > m_maxValue) {
        value = m_maxValue;
    }
    if (qFuzzyCompare(m_value, value))
        return;
    m_value = value;
    update();
    emit valueChanged(m_value);
}

void VerticalIndicator::setMaxValue(qreal maxValue)
{
    if (qFuzzyCompare(m_maxValue, maxValue))
        return;
    m_maxValue = maxValue;
    update();
    emit maxValueChanged(m_maxValue);
}

void VerticalIndicator::setStartColor(const QColor &color)
{
    if (m_startColor == color)
        return;
    m_startColor = color;
    update();
    emit startColorChanged(m_startColor);
}

void VerticalIndicator::setMidColor(const QColor &color)
{
    if (m_midColor == color)
        return;
    m_midColor = color;
    update();
    emit midColorChanged(m_midColor);
}

void VerticalIndicator::setEndColor(const QColor &color)
{
    if (m_endColor == color)
        return;
    m_endColor = color;
    update();
    emit endColorChanged(m_endColor);
}

void VerticalIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw background
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::lightGray);
    painter.drawRoundedRect(rect(), 5, 5);

    // Create full gradient based on max value
    QLinearGradient gradient(rect().bottomLeft(), rect().topLeft());
    gradient.setColorAt(0, m_startColor);
    gradient.setColorAt(0.5, m_midColor);
    gradient.setColorAt(1, m_endColor);

    // Calculate the height of the visible part based on current value
    int visibleHeight = static_cast<int>((m_value / m_maxValue) * rect().height());
    QRect visibleRect = rect();
    visibleRect.setHeight(visibleHeight);
    visibleRect.moveBottom(rect().bottom());

    // Draw the colored part
    painter.setBrush(gradient);
    painter.setClipRect(visibleRect);
    painter.drawRoundedRect(rect(), 5, 5);
}