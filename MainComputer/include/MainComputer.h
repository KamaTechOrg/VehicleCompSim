#ifndef MAINCOMPUTER_H
#define MAINCOMPUTER_H

#include "ConditionsManager.h"
#include "Editor.h"
#include "json.hpp"

class MainComputer
{
    Editor editor;
    ConditionsManager manager;
public:
    MainComputer();

    void openEditor();
    void run();
    void stop();

    nlohmann::json guiData, logicData;
};

#endif // MAINCOMPUTER_H
