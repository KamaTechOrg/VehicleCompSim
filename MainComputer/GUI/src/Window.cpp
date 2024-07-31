#include "Window.h"

#include <iostream>
#include <fstream>

Window::Window()
{
    setWindowTitle("Main Computer");
    
    _ConditionsBox = new ConditionsBox;
    _thenGroupBox = new ThenGroupBox;

    _saveButton = new QPushButton("save");
    connect(_saveButton, &QPushButton::clicked, this, &Window::save);

    _windowLayout = new QVBoxLayout;
    _windowLayout->addWidget(_ConditionsBox);
    _windowLayout->addWidget(_thenGroupBox);
    _windowLayout->addStretch(1);
    _windowLayout->addWidget(_saveButton);

    setLayout(_windowLayout);
}

void Window::save()
{
    std::string filename = "conditions.json";
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    nlohmann::json j;
    j = _ConditionsBox->data()->toJson();

    file << j.dump(4);
    file.close();
}
