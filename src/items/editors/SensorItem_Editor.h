#ifndef SENSORITEM_EDITOR_H
#define SENSORITEM_EDITOR_H
#include "../sensoritem.h"
#include "qcheckbox.h"

#include "gui/editpanel.h"
#include "qfiledialog.h"
#include "qfileselector.h"
#include "qlineedit.h"
#include "qpushbutton.h"

class SensorItem::Editor : public EditPanel::Editor{
    SensorModel& model;

    QLineEdit* priority = new QLineEdit(this);
    QLineEdit* name = new QLineEdit(this);
    QLineEdit* buildCommand = new QLineEdit(this);
    QLineEdit* runCommand = new QLineEdit(this);
    QPushButton* cmakeSelectorOpen = new QPushButton(this);
    QCheckBox* isUseCmakePath = new QCheckBox(this);

protected:

    void initPriority();
    void initName();
    void initBuildCommand();
    void initRunCommand();
    void initCmakeSelectorOpen();
    void initIsUseCmakePath();


    void initParameters(); // Declare as virtual
    void initLayout();      // Declare as virtual

    QVBoxLayout* layout = new QVBoxLayout(this);

    std::unordered_map<QObject*, QLabel*> labels;
    void switchProjectInputMethod();
public:
    Editor(SensorItem* _sensor);
    void open();
    void close();
};
#endif // SENSORITEM_EDITOR_H
