#ifndef MAINCOMPUTER_H
#define MAINCOMPUTER_H

#include "ConditionsManager.h"
#include "Editor.h"
#include "json.hpp"

class MainComputer
{
    std::shared_ptr<Editor> editor;
    ConditionsManager manager;
public:
    MainComputer(std::function<void(void)> _onDataChanged = [](){});

    void openEditor();
    void run();
    void stop();
    std::function<void(void)> onDataChanged;
    nlohmann::json guiData, logicData;
};

#endif // MAINCOMPUTER_H
