#include "MainComputer.h"

MainComputer::MainComputer(std::function<void(void)> _onDataChanged)
    : editor(nullptr), onDataChanged(_onDataChanged)
{}

void MainComputer::openEditor()
{
    editor.reset(new Editor(guiData, logicData));
    editor->show();
    //guiData = editor.guiData;
    //logicData = editor.logicData;
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
