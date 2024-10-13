#include "ThenGroupBox.h"
#include <QMessageBox>


ThenGroupBox::ThenGroupBox(QWidget* parent)
    : QGroupBox("then", parent)
{
    QFont font = this->font();
    font.setBold(true);
    setFont(font);
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
    bool hasEmptyField = false;

    for (auto layout : _actionLayouts) {
        auto action = layout->data();
        if (!action) {
            hasEmptyField = true; 
            break; 
        }
        actions.push_back(action);
    }

    if (hasEmptyField) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("Validation Error"));
        msgBox.setText(tr("Please make sure all fields are filled out for all actions."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        return {};
    }

    return actions;
}


void ThenGroupBox::setView(const nlohmann::json& json)
{
    for (auto layout : _actionLayouts)
        removeActionLayout(layout);

    for (const auto& actionJson : json)
    {
        addActionLayout();
        _actionLayouts.back()->loadFromJson(actionJson);
    }
}

nlohmann::json ThenGroupBox::GuiData()
{
    nlohmann::json actionsJson = nlohmann::json::array();
    for (auto layout : _actionLayouts) {
        nlohmann::json actionData = layout->GuiData();
        if (!actionData.is_null()) {
            actionsJson.push_back(actionData);
        }
    }
    return actionsJson;
}
