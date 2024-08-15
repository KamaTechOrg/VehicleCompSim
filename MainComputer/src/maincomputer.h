#ifndef MAINCOMPUTER_H
#define MAINCOMPUTER_H

#include "ConditionsManager.h"
#include "Window.h"

class MainComputer
{
    Window editor;
    ConditionsManager manager;
public:
    MainComputer();

    void openEditor();
    void run();
    void stop();
};

#endif // MAINCOMPUTER_H
