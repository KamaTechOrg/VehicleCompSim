#pragma once

#include <QPushButton>
#include <QIcon>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include "globalconstants.h"

class IconButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
    explicit IconButton(globalConstants::Icons icon, const QString& toolTip, QWidget* parent = nullptr);

    void changeIcon(globalConstants::Icons icon);
    void setCustomSize(const QSize& size);

    qreal scale() const { return m_scale; }
    void setScale(qreal scale);

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void setupButton(const QString& iconPath, const QString& toolTip);
    void setupAnimation();
    void updateStyleSheet();

    QPropertyAnimation* m_scaleAnimation;
    qreal m_scale;
    QSize m_baseSize;
};