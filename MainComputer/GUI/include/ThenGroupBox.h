#pragma once

#include <QGroupBox>

#include "ThenWidgetsLayout.h"
#include "Action.h"

class ThenGroupBox : public QGroupBox
{
public:
	ThenGroupBox(QWidget* parent = nullptr);

	std::shared_ptr<Action> data();

private:
	ThenWidgetsLayout* _thenLayout;
};