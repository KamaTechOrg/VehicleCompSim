#include "verticalindicator.h"
#include "globalstate.h"

VerticalIndicator::VerticalIndicator(QWidget *parent)
    : QWidget(parent), m_value(0), m_maxValue(100),
      m_startColor(Qt::green), m_midColor(Qt::yellow), m_endColor(Qt::red)
{
    setFixedWidth(10);
    setFixedHeight(90);
    connect(&GlobalState::getInstance(), &GlobalState::maxMessageCountChanged, this, &VerticalIndicator::onMaxValueChanged);
}

qreal VerticalIndicator::value() const { return m_value; }
qreal VerticalIndicator::maxValue() const { return m_maxValue; }

void VerticalIndicator::incrementValue()
{
    qreal newValue = m_value + 1;
    if (qFuzzyCompare(m_value, newValue))
        return;
    if (newValue > m_maxValue) {
        GlobalState::getInstance().setMaxMessageCount(m_maxValue + m_maxValue / 2);
    }
    if (newValue < 0) {
        newValue = 0;
    }
    m_value = newValue;
    update();
}

void VerticalIndicator::setMaxValue(qreal maxValue)
{
    if (qFuzzyCompare(m_maxValue, maxValue))
        return;
    m_maxValue = maxValue;
    update();
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

void VerticalIndicator::onMaxValueChanged(qreal maxValue)
{
    setMaxValue(maxValue);
}