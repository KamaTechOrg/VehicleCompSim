#include "ConditionsEditor.h"

#include <iostream>
#include <fstream>

#include "constants.h"
#include "ConditionsManager.h"
#include "JsonLoader.h"

ConditionsEditor::ConditionsEditor()
    : QGroupBox("Editor")
{
    _conditionsGroup = new ConditionsGroup;
    _conditionsGroup->unableDelete();
    _conditionsGroup->setBoxTitle("if");

    _actionGroupBox = new ThenGroupBox;

    _saveButton = new QPushButton("save");
    connect(_saveButton, &QPushButton::clicked, this, &ConditionsEditor::save);

    _ConditionsEditorLayout = new QVBoxLayout;
    _ConditionsEditorLayout->addLayout(_conditionsGroup);
    _ConditionsEditorLayout->addWidget(_actionGroupBox);
    _ConditionsEditorLayout->addStretch(1);
    _ConditionsEditorLayout->addWidget(_saveButton);
    
    setLayout(_ConditionsEditorLayout);

    resize(350, 200);
}

void ConditionsEditor::setView(nlohmann::json jsonData)
{
    _conditionsGroup->setConditionsGroup(jsonData["conditions"]);
    _actionGroupBox->setView(jsonData["actions"]);
}

void ConditionsEditor::save()
{
    saveLogicDataToJson();
    saveGuiDataToJson();
}

void ConditionsEditor::showSaveFeedback(bool success)
{
    if (success) {
        _saveButton->setStyleSheet("background-color: #4CAF50;"); // Green
    }
    else {
        _saveButton->setStyleSheet("background-color: #F44336;"); // Red
    }

    QTimer::singleShot(3000, [this]() {
        _saveButton->setStyleSheet("");
        });
}

void ConditionsEditor::loadGuiDataFromJson()
{
    nlohmann::json jsonData = JsonLoader().loadGuiData();

    _conditionsGroup->addConditionsGroup(jsonData["conditions"]);
    _actionGroupBox->setView(jsonData["actions"]);
}

void ConditionsEditor::saveLogicDataToJson() {
    std::shared_ptr<ConditionBase> conditionsTree = _conditionsGroup->logicData();
    if (conditionsTree == nullptr)
        return;

    // get action (that will be executed if conditon were validated at some point)
    std::vector<std::shared_ptr<Action>> actions = _actionGroupBox->data();
    if (actions.empty())
        return;

    nlohmann::json jsonData;
    jsonData["conditions"] = conditionsTree->toJson();
    jsonData["actions"] = nlohmann::json::array();
    for (const auto& action : actions) {
        jsonData["actions"].push_back(action->toJson());
    }

    JsonLoader().saveConditionsLogic(jsonData);
    showSaveFeedback(true);

    ConditionsManager().loadFromJson();
}

void ConditionsEditor::saveGuiDataToJson() {
    nlohmann::json jsonData;

    jsonData["conditions"] = _conditionsGroup->GuiData();
    jsonData["actions"] = _actionGroupBox->GuiData();

    JsonLoader().saveGuiData(jsonData);
}