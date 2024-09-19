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

    QString cmakePath;

protected:

    QVBoxLayout* layout = new QVBoxLayout(this);
    std::unordered_map<QObject*, QLabel*> labels;

    void initPriority();
    void initName();
    void initBuildCommand();
    void initRunCommand();
    void initCmakeSelectorOpen();
    void initIsUseCmakePath();

    void initSaveCancelBtns();
    void initParameters();
    void initLayout();

    void onSaveBtnClicked();
    void onCancelBtnCliked();
    void switchProjectInputMethod();

public:

    Editor(SensorModel* _sensor);
    void open();
    void close();
};
#endif // SENSORITEM_EDITOR_H
