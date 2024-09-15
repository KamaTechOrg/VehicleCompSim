#ifndef EDITPANEL_H
#define EDITPANEL_H

#include "qtoolbar.h"

class EditPanel
{
public:
    //EditPanel() = delete;

    class Editor : public QWidget {
    public:
        virtual void open() = 0;
        virtual void close() = 0;
    };
    static void loadNewEditor(Editor* editor);
    static void closeCurrentEditor();
    static QToolBar *getPanel();

private:
    static Editor* currentEditor;

};

#endif // EDITPANEL_H
