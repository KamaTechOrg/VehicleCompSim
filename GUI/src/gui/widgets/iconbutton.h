#pragma once

#include <QPushButton>
#include <QIcon>

#include "globalconstants.h"

class IconButton : public QPushButton
{
    Q_OBJECT

public:
    explicit IconButton(globalConstants::Icons icon, const QString& toolTip, QWidget* parent = nullptr);

    // Method to change the icon
    void changeIcon(globalConstants::Icons icon);

private:
    void setupButton(const QString& iconPath, const QString& toolTip);
};