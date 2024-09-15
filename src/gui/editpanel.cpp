#include "editpanel.h"


void EditPanel::loadNewEditor(EditPanel::Editor *editor)
{
    if (currentEditor != nullptr) closeCurrentEditor();

    currentEditor = editor;
    currentEditor->setParent(getPanel());
    currentEditor->show();
}

void EditPanel::closeCurrentEditor()
{
    currentEditor->close();
    currentEditor->deleteLater();
    currentEditor = nullptr;
}

QToolBar *EditPanel::getPanel()
{
    static QToolBar* panel = new QToolBar("rightPanel");
    return panel;
}

EditPanel::Editor* EditPanel::currentEditor = nullptr;
