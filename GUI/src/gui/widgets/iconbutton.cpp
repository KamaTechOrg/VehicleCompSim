#include "iconbutton.h"

IconButton::IconButton(globalConstants::Icons icon, const QString& toolTip, QWidget* parent)
    : QPushButton(parent)
{
    setupButton(globalConstants::getIconPath(icon), toolTip);
}

void IconButton::setupButton(const QString& iconPath, const QString& toolTip)
{
    setIcon(QIcon(iconPath));
    setToolTip(toolTip);
    setStyleSheet("QPushButton { border: none; background: none; }");
    setFixedSize(50, 50);
    setIconSize(QSize(50, 50));
}

void IconButton::changeIcon(globalConstants::Icons icon)
{
    QPushButton::setIcon(QIcon(globalConstants::getIconPath(icon)));
}