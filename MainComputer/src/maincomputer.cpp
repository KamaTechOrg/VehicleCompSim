#include "MainComputer.h"

MainComputer::MainComputer() : editor(guiData, logicData) {}

void MainComputer::openEditor()
{
    editor.show();
    guiData = editor.guiData;
    logicData = editor.logicData;
}

void MainComputer::run()
{
    manager.jsonData = logicData;
    manager.run();
}

void MainComputer::stop()
{
    manager.stop();
}
