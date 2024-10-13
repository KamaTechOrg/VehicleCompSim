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
    QFont font = this->font();
    font.setBold(true);
    setFont(font);
    _actionGroupBox = new ThenGroupBox;
    _ConditionsEditorLayout = new QVBoxLayout;
    _ConditionsEditorLayout->addLayout(_conditionsGroup);
    _ConditionsEditorLayout->addWidget(_actionGroupBox);
    _ConditionsEditorLayout->addStretch(1);
    setLayout(_ConditionsEditorLayout);
    setMinimumWidth(520);
}

void ConditionsEditor::setView(nlohmann::json jsonData)
{
    if (jsonData.contains("conditions"))
        _conditionsGroup->setConditionsGroup(jsonData["conditions"]);
    if (jsonData.contains("actions"))
        _actionGroupBox->setView(jsonData["actions"]);
}

void ConditionsEditor::loadGuiDataFromJson()
{
    nlohmann::json jsonData = JsonLoader().loadGuiData();
    _conditionsGroup->addConditionsGroup(jsonData["conditions"]);
    _actionGroupBox->setView(jsonData["actions"]);
}

nlohmann::json ConditionsEditor::getLogicDataAsJson() {

    std::shared_ptr<ConditionBase> conditionsTree = _conditionsGroup->logicData();
    if (conditionsTree == nullptr)
        return nlohmann::json();

    std::vector<std::shared_ptr<Action>> actions = _actionGroupBox->data();
    if (actions.empty())
        return nlohmann::json();

    nlohmann::json jsonData;
    jsonData["conditions"] = conditionsTree->toJson();
    jsonData["actions"] = nlohmann::json::array();
    for (const auto& action : actions) {
        jsonData["actions"].push_back(action->toJson());
    }
    return jsonData;
}

nlohmann::json ConditionsEditor::getGuiDataAsJson() {
    nlohmann::json jsonData;
    jsonData["conditions"] = _conditionsGroup->GuiData();
    jsonData["actions"] = _actionGroupBox->GuiData();
    return jsonData;
}