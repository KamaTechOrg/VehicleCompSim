#include "ConditionsEditor.h"

#include <iostream>
#include <fstream>

#include "constants.h"
#include "conditionsManager.h"

ConditionsEditor::ConditionsEditor()
    : QGroupBox("Editor")
{
    _conditionsGroup = new ConditionsGroup;
    _conditionsGroup->unableDelete();
    _conditionsGroup->setBoxTitle("if");
    _thenGroupBox = new ThenGroupBox;

    _saveButton = new QPushButton("save");
    connect(_saveButton, &QPushButton::clicked, this, &ConditionsEditor::save);

    _ConditionsEditorLayout = new QVBoxLayout;
    _ConditionsEditorLayout->addLayout(_conditionsGroup);
    _ConditionsEditorLayout->addWidget(_thenGroupBox);
    _ConditionsEditorLayout->addStretch(1);
    _ConditionsEditorLayout->addWidget(_saveButton);

    setLayout(_ConditionsEditorLayout);


    resize(350, 200);

    // load from JSON file (if exists) the current conditions state
    loadGuiDataFromJson(constants::GUI_DATA_JSON_FILE_NAME);
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

void ConditionsEditor::loadGuiDataFromJson(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        qDebug() << "Failed to open file: " << QString::fromStdString(filename);
        return;
    }

    nlohmann::json jsonData;
    try {
        file >> jsonData;
    }
    catch (const nlohmann::json::parse_error& e) {
        qDebug() << "JSON parse error: " << e.what();
        file.close();
        return;
    }
    file.close();

    qDebug() << "Loaded JSON data: " << QString::fromStdString(jsonData.dump(4));

    // Check if the keys exist in the JSON data
    if (jsonData.contains("conditions") && jsonData.contains("actions")) {
        _conditionsGroup->addConditionsGroup(jsonData["conditions"]);
        _thenGroupBox->loadFromJson(jsonData["actions"]);
    }
    else {
        qDebug() << "JSON data missing expected keys: 'conditions' or 'actions'.";
    }
}

void ConditionsEditor::saveLogicDataToJson() {
    std::shared_ptr<ConditionBase> conditionsTree = _conditionsGroup->logicData();

    if (conditionsTree == nullptr)
        return;

    // get action (that will be executed if conditon were validated at some point)
    std::vector<std::shared_ptr<Action>> actions = _thenGroupBox->data();

    
    if (actions.empty())
        return;

    // save both conditions and action to a json file
    std::string jsonFileName = constants::CONDITIONS_JSON_FILE_NAME;

    std::ofstream jsonFile(jsonFileName);
    if (!jsonFile.is_open()) {
        showSaveFeedback(false);
        QMessageBox::critical(this, "Error", "Cannot open file: " + QString::fromStdString(jsonFileName));
        return;
    }

    nlohmann::json jsonData;
    jsonData["conditions"] = conditionsTree->toJson();
    jsonData["actions"] = nlohmann::json::array();
    for (const auto& action : actions) {
        jsonData["actions"].push_back(action->toJson());
    }
    jsonFile << jsonData.dump(4);
    jsonFile.close();

    showSaveFeedback(true);

    ConditionsManager().loadFromJson(jsonFileName);
}

void ConditionsEditor::saveGuiDataToJson() {
    nlohmann::json jsonData;

    jsonData["conditions"] = _conditionsGroup->GuiData();
    jsonData["actions"] = _thenGroupBox->GuiData();


    // save both conditions and action to a json file
    std::string jsonFileName = constants::GUI_DATA_JSON_FILE_NAME;

    std::ofstream jsonFile(jsonFileName);
    if (!jsonFile.is_open()) {
        return;
    }

    jsonFile << jsonData.dump(4);
    jsonFile.close();
}