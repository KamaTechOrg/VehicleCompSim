#ifndef EDITPANEL_H
#define EDITPANEL_H

#include "qtoolbar.h"
#include "qscrollarea.h"

class EditPanel
{
public:

    class Editor : public QWidget {
    public:
        virtual void open() = 0;
        virtual void close() = 0;
    };
    static void loadNewEditor(Editor* editor);
    static void closeCurrentEditor();
    static QToolBar *getPanel();
    static QWidget *getScrollArea();

private:
    EditPanel();
    Editor* currentEditor;
    QToolBar *panel;
    QScrollArea* scrollArea;

    static EditPanel& getInstance();

};

#endif // EDITPANEL_H
