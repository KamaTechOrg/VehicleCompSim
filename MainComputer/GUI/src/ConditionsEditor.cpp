#include "ConditionsEditor.h"

#include <iostream>
#include <fstream>

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
}

void ConditionsEditor::save()
{
    std::string filename = "conditions.json";
   // std::string filename = "C:/Windows/conditions.json"; 

    std::ofstream file(filename);
    if (!file.is_open()) {
        showSaveFeedback(false);
        QMessageBox::critical(this, "Error", "Cannot open file: " + QString::fromStdString(filename));
        return;
    }

    std::shared_ptr<ConditionBase> conditionsTree = _conditionsGroup->data();
    nlohmann::json j;
    if (conditionsTree != nullptr)
    {
        j = conditionsTree->toJson();
    }
    else
    {
        j = {};
    }
    
    file << j.dump(4);
    file.close();
    showSaveFeedback(true);
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