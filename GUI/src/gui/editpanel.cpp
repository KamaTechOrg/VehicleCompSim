#include "editpanel.h"
#include "qemusensormodel.h"
#include "maincomputermodel.h"
#include "editors/QemuSensorItem_Editor.h"
#include "editors/SensorItem_Editor.h"
#include "editors/MainComputer_Editor.h"
#include "editors/tester3.h"

void EditPanel::loadNewEditor(EditPanel::Editor *editor)
{
    closeCurrentEditor();

    currentEditor = editor;
    panel->addWidget( editor);
    editor->open();
}

void EditPanel::closeCurrentEditor()
{
    if (currentEditor)
    {
        currentEditor->close();
        currentEditor->deleteLater();
        currentEditor = nullptr;
    }
}

QToolBar *EditPanel::getPanel()
{
    return getInstance().panel;
}

EditPanel::EditPanel():
    panel(new QToolBar),
    globalState(GlobalState::getInstance())
{
    currentModel = globalState.currentSensorModel();
    QObject::connect(&globalState, &GlobalState::currentSensorModelChanged, [this](){onGlobalStateCurrentSensorModelChanged();});

}

void EditPanel::onGlobalStateCurrentSensorModelChanged()
{
    currentModel = globalState.currentSensorModel();
    if (currentModel) {
        loadNewEditor(getEditorForModel(currentModel));
    }
    else closeCurrentEditor();
}

EditPanel::Editor *EditPanel::getEditorForModel(SensorModel *model)
{

    if (!model) return nullptr;
    if (dynamic_cast<MainComputerModel*>(model))
    {
        return new MainComputerItem::Editor(dynamic_cast<MainComputerModel*>(model));
    }
    if (dynamic_cast<QemuSensorModel*>(model))
    {
        return new QemuSensorItem::Editor(dynamic_cast<QemuSensorModel*>(model));
    }
    else
    {
        return new SensorItem::Editor(model);
    }
}

EditPanel &EditPanel::getInstance()
{
    static EditPanel instance;
    return instance;
}
