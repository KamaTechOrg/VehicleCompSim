#include "ThenGroupBox.h"

ThenGroupBox::ThenGroupBox(QWidget* parent)
	: QGroupBox("then", parent)
{
	_thenLayout = new ThenWidgetsLayout;
	setLayout(_thenLayout);
}

std::shared_ptr<Action> ThenGroupBox::data()
{
	return _thenLayout->data();
}