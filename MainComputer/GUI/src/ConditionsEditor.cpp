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
    loadDataFromJson(constants::CONDITIONS_JSON_FILE_NAME);
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

void ConditionsEditor::loadDataFromJson(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        return; // in this case, we just don't load anything

    nlohmann::json jsonData;
    file >> jsonData;
    file.close();

    /*
    * TODO: If we did found an existing JSON file.
    * then we need to load it's data,
    * and set the initial input in the conditins GUI to be the data from the file
    */
}

void ConditionsEditor::saveLogicDataToJson() {
    std::shared_ptr<ConditionBase> conditionsTree = _conditionsGroup->logicData();

    if (conditionsTree == nullptr)
        return;

    // get action (that will be executed if conditon were validated at some point)
    std::shared_ptr<Action> action = _thenGroupBox->data();

    if (action == nullptr)
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
    jsonData["action"] = action->toJson();
    jsonFile << jsonData.dump(4);
    jsonFile.close();

    showSaveFeedback(true);

    // get the "backend" main computer to reload again the conditions from the saved JSON file
    ConditionsManager().loadFromJson(jsonFileName);
}

void ConditionsEditor::saveGuiDataToJson() {
    nlohmann::json jsonData;

    jsonData = _conditionsGroup->GuiData();

    // save both conditions and action to a json file
    std::string jsonFileName = constants::GUI_DATA_JSON_FILE_NAME;

    std::ofstream jsonFile(jsonFileName);
    if (!jsonFile.is_open()) {
        return;
    }

    jsonFile << jsonData.dump(4);
    jsonFile.close();
}