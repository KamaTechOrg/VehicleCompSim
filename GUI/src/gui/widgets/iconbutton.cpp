#include "iconbutton.h"
#include <QEnterEvent>
#include <QResizeEvent>

IconButton::IconButton(globalConstants::Icons icon, const QString& toolTip, QWidget* parent)
    : QPushButton(parent), m_scale(1.0), m_baseSize(50, 50)
{
    setupButton(globalConstants::getIconPath(icon), toolTip);
    setupAnimation();
}

void IconButton::setupButton(const QString& iconPath, const QString& toolTip)
{
    setIcon(QIcon(iconPath));
    setToolTip(toolTip);
    updateStyleSheet();
    setFixedSize(m_baseSize);
    setIconSize(m_baseSize * 0.8);
}

void IconButton::setupAnimation()
{
    m_scaleAnimation = new QPropertyAnimation(this, "scale", this);
    m_scaleAnimation->setDuration(200);
    m_scaleAnimation->setEasingCurve(QEasingCurve::InOutQuad);
}

void IconButton::setScale(qreal scale)
{
    m_scale = scale;
    setIconSize(m_baseSize * 0.8 * scale);
}

void IconButton::enterEvent(QEnterEvent* event)
{
    if (isEnabled()) {
        m_scaleAnimation->setStartValue(1.0);
        m_scaleAnimation->setEndValue(1.2);
        m_scaleAnimation->start();
    }
    QPushButton::enterEvent(event);
}

void IconButton::leaveEvent(QEvent* event)
{
    if (isEnabled()) {
        m_scaleAnimation->setStartValue(1.2);
        m_scaleAnimation->setEndValue(1.0);
        m_scaleAnimation->start();
    }
    QPushButton::leaveEvent(event);
}

void IconButton::changeIcon(globalConstants::Icons icon)
{
    setIcon(QIcon(globalConstants::getIconPath(icon)));
}

void IconButton::setCustomSize(const QSize& size)
{
    m_baseSize = size;
    setFixedSize(m_baseSize);
    setIconSize(m_baseSize * 0.8);
    updateStyleSheet();
}

void IconButton::resizeEvent(QResizeEvent* event)
{
    m_baseSize = event->size();
    setIconSize(m_baseSize * 0.8);
    updateStyleSheet();
    QPushButton::resizeEvent(event);
}

void IconButton::updateStyleSheet()
{
    setStyleSheet(QString(
        "QPushButton { border: none; background: none; }"
        "QPushButton:hover { background-color: rgba(200, 200, 200, 50); border-radius: %1px; }"
    ).arg(m_baseSize.width() / 2));
}