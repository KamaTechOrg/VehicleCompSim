#include "MainComputer_Editor.h"

MainComputerItem::Editor::Editor(MainComputerModel *_model) : model(*_model)
{

}

void MainComputerItem::Editor::open()
{
    model.computer().openEditor();
    if (GlobalState::getInstance().currentSensorModel() == &model)
    {
        GlobalState::getInstance().setCurrentSensorModel(nullptr);
    }
}

void MainComputerItem::Editor::close()
{
    GlobalState::getInstance().log("closing main computer editor", "Terminal");
}
