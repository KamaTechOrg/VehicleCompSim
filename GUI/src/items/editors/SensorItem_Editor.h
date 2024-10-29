#ifndef SENSORITEM_EDITOR_H
#define SENSORITEM_EDITOR_H
#include "../sensoritem.h"
#include "qcheckbox.h"

#include "gui/editpanel.h"
#include "qfiledialog.h"
#include "qfileselector.h"
#include "qlineedit.h"
#include "qpushbutton.h"
class CCV{};
class SensorItem::Editor : public EditPanel::Editor{
    SensorModel& model;
    QVBoxLayout* mainLayout;
    QWidget* contentWidget;

    QLineEdit* priority;
    QLineEdit* name;
    QLineEdit* buildCommand;
    QLineEdit* runCommand;
    QPushButton* cmakeSelectorOpen;
    QCheckBox* isUseCmakePath;

    QString cmakePath;

protected:

    std::unordered_map<QObject*, QLabel*> labels;

    void initPriority();
    void initName();
    void initBuildCommand();
    void initRunCommand();
    void initCmakeSelectorOpen();
    void initIsUseCmakePath();

    void initSaveCancelBtns();
    void initParameters();
    void initUI();

    void onSaveBtnClicked();
    void onCancelBtnCliked();
    void switchProjectInputMethod();

public:

    Editor(SensorModel* _sensor);
    void open();
    void close();
};
#endif // SENSORITEM_EDITOR_H
