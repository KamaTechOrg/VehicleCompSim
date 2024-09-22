#include "editpanel.h"
#include "qemusensormodel.h"
#include "editors/QemuSensorItem_Editor.h"
#include"editors/SensorItem_Editor.h"

void EditPanel::loadNewEditor(EditPanel::Editor *editor)
{
    closeCurrentEditor();

    getInstance().currentEditor = editor;
    getInstance().panel->addWidget( editor);
    //getInstance().scrollArea->setWidget(editor);
    editor->open();
    //editor->setFixedSize(getInstance().getScrollArea()->size());
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
QWidget *EditPanel::getScrollArea()
{
    return getInstance().scrollArea;
}

EditPanel::EditPanel():
    panel(new QToolBar),
    scrollArea (new QScrollArea(/*panel*/)),
    globalState(GlobalState::getInstance())
{
    currentModel = globalState.currentSensorModel();
    QObject::connect(&globalState, &GlobalState::currentSensorModelChanged, [this](){onGlobalStateCurrentSensorModelChanged();});
    //scrollArea->setWidgetResizable(true);
    //scrollArea->setFixedWidth(250);
    //scrollArea->setFixedHeight(panel->height());
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
