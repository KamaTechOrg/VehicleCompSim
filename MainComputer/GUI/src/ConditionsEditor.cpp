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

}

void ConditionsEditor::save()
{
    std::shared_ptr<ConditionBase> conditionsTree = _conditionsGroup->data();

    if (conditionsTree == nullptr) {
        return;
    }

    std::string filename = CONDITIONS_JSON_FILE_NAME;

    std::ofstream file(filename);
    if (!file.is_open()) {
        showSaveFeedback(false);
        QMessageBox::critical(this, "Error", "Cannot open file: " + QString::fromStdString(filename));
        return;
    }

    nlohmann::json j;
    j = conditionsTree->toJson();

    file << j.dump(4);
    file.close();
    showSaveFeedback(true);

    ConditionsManager().loadFromJson(filename);
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