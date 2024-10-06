#ifndef EDITPANEL_H
#define EDITPANEL_H

#include "qtoolbar.h"
#include "qscrollarea.h"
#include "sensormodel.h"
#include "globalstate.h"

class EditPanel
{
public:

    class Editor : public QWidget {
    public:
        virtual void open() = 0;
        virtual void close() = 0;
    };
    static QToolBar *getPanel();

private:
    EditPanel();
    Editor* currentEditor;
    QToolBar *panel;
    SensorModel* currentModel;
    GlobalState& globalState;

    void onGlobalStateCurrentSensorModelChanged();
    Editor* getEditorForModel(SensorModel* model);

    void loadNewEditor(Editor* editor);
    void closeCurrentEditor();
    static EditPanel& getInstance();

};

#endif // EDITPANEL_H
