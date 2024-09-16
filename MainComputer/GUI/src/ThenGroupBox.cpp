#include "ThenGroupBox.h"


ThenGroupBox::ThenGroupBox(QWidget* parent)
    : QGroupBox("then", parent)
{
    _mainLayout = new QVBoxLayout;
    setLayout(_mainLayout);

    _addActionButton = new QPushButton("+");
    int defaultHeight = _addActionButton->sizeHint().height();
    _addActionButton->setFixedSize(defaultHeight, defaultHeight);
    connect(_addActionButton, &QPushButton::clicked, this, &ThenGroupBox::addActionLayout);

    _mainLayout->addWidget(_addActionButton);
}

void ThenGroupBox::addActionLayout()
{
    ThenWidgetsLayout* actionLayout = new ThenWidgetsLayout;
    _actionLayouts.push_back(actionLayout);

    connect(actionLayout, &ThenWidgetsLayout::requestDelete, this, &ThenGroupBox::removeActionLayout);

    // Cast actionLayout to QWidget* for insertWidget
    _mainLayout->insertWidget(_mainLayout->count() - 1, static_cast<QWidget*>(actionLayout));
}

void ThenGroupBox::removeActionLayout(ThenWidgetsLayout* layout)
{
    auto it = std::find(_actionLayouts.begin(), _actionLayouts.end(), layout);
    if (it != _actionLayouts.end()) {
        _actionLayouts.erase(it);
        _mainLayout->removeWidget(layout);
        delete layout;
    }
}

std::vector<std::shared_ptr<Action>> ThenGroupBox::data()
{
    std::vector<std::shared_ptr<Action>> actions;
    for (auto layout : _actionLayouts) {
        auto action = layout->data();
        if (action) {
            actions.push_back(action);
        }
    }
    return actions;
}

void ThenGroupBox::loadFromJson(const nlohmann::json& json)
{
    for (const auto& actionJson : json) {
        addActionLayout();
        _actionLayouts.back()->loadFromJson(actionJson);
    }
}

nlohmann::json ThenGroupBox::GuiData()
{
    nlohmann::json actionsJson = nlohmann::json::array();
    for (auto layout : _actionLayouts) {
        actionsJson.push_back(layout->GuiData());
    }
    return actionsJson;
}
