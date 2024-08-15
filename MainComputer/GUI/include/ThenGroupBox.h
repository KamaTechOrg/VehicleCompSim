#pragma once

#include <QGroupBox>

#include "ThenWidgetsLayout.h"

class ThenGroupBox : public QGroupBox
{
public:
	ThenGroupBox(QWidget* parent = nullptr);

private:
	ThenWidgetsLayout* _thenLayout;
};