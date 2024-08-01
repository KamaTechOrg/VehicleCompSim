#include "ConditionsEditor.h"

#include <iostream>
#include <fstream>

ConditionsEditor::ConditionsEditor()
    : QGroupBox("Editor")
{
    _ConditionsBox = new ConditionsBox;
    _thenGroupBox = new ThenGroupBox;

    _saveButton = new QPushButton("save");
    connect(_saveButton, &QPushButton::clicked, this, &ConditionsEditor::save);

    _ConditionsEditorLayout = new QVBoxLayout;
    _ConditionsEditorLayout->addWidget(_ConditionsBox);
    _ConditionsEditorLayout->addWidget(_thenGroupBox);
    _ConditionsEditorLayout->addStretch(1);
    _ConditionsEditorLayout->addWidget(_saveButton);

    setLayout(_ConditionsEditorLayout);
}

void ConditionsEditor::save()
{
    std::string filename = "conditions.json";
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::shared_ptr<ConditionBase> conditionsTree = _ConditionsBox->data();
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
}
