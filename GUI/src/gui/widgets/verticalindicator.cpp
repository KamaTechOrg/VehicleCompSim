#include "verticalindicator.h"
#include "globalstate.h"

VerticalIndicator::VerticalIndicator(QWidget *parent)
    : QWidget(parent), m_value(0), m_maxValue(50),
      m_startColor(Qt::green), m_midColor(Qt::yellow), m_endColor(Qt::red)
{
    setFixedWidth(50);
    setFixedHeight(110);
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

    // Set the widget's background to be transparent
    setAttribute(Qt::WA_TranslucentBackground, true);

    // Define the fixed width for the indicator
    int indicatorWidth = 10;

    // Draw background (transparent)
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    QRect indicatorRect(0, 0, indicatorWidth, height() - 20); // Leave space for the text
    painter.drawRoundedRect(indicatorRect, 5, 5);

    // Create full gradient based on max value
    QLinearGradient gradient(indicatorRect.bottomLeft(), indicatorRect.topLeft());
    gradient.setColorAt(0, m_startColor);
    gradient.setColorAt(0.5, m_midColor);
    gradient.setColorAt(1, m_endColor);

    // Calculate the height of the visible part based on current value
    int visibleHeight = static_cast<int>((m_value / m_maxValue) * indicatorRect.height());
    QRect visibleRect = indicatorRect;
    visibleRect.setHeight(visibleHeight);
    visibleRect.moveBottom(indicatorRect.bottom());

    // Draw the colored part
    painter.setBrush(gradient);
    painter.setClipRect(visibleRect);
    painter.drawRoundedRect(indicatorRect, 5, 5);

    // Draw the value text to the right of the indicator
    painter.setClipping(false); // Disable clipping
    painter.setPen(Qt::black);
    QString valueText = QString::number(m_value);
    QFontMetrics metrics(painter.font());
    int textWidth = metrics.horizontalAdvance(valueText);
    QRect textRect(0, height() - 20, textWidth, metrics.height());
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, valueText);
}
void VerticalIndicator::onMaxValueChanged(qreal maxValue)
{
    setMaxValue(maxValue);
}