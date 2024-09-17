#include "editpanel.h"


void EditPanel::loadNewEditor(EditPanel::Editor *editor)
{
    if (getInstance().currentEditor != nullptr) closeCurrentEditor();

    getInstance().currentEditor = editor;
    getInstance().panel->addWidget( editor);
    //getInstance().scrollArea->setWidget(editor);
    editor->open();
    //editor->setFixedSize(getInstance().getScrollArea()->size());
}

void EditPanel::closeCurrentEditor()
{
    getInstance().currentEditor->close();
    getInstance().currentEditor->deleteLater();
    getInstance().currentEditor = nullptr;
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
    scrollArea (new QScrollArea(/*panel*/))
{
    //scrollArea->setWidgetResizable(true);
    //scrollArea->setFixedWidth(250);
    //scrollArea->setFixedHeight(panel->height());
}

EditPanel &EditPanel::getInstance()
{
    static EditPanel instance;
    return instance;
}
