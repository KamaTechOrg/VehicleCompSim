#ifndef QEMUSENSORITEM_EDITOR_H
#define QEMUSENSORITEM_EDITOR_H
#include "../qemusensoritem.h"
#include "../gui/editpanel.h"
#include "qcheckbox.h"
#include "qcombobox.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qspinbox.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>


class QemuSensorItem::Editor : public EditPanel::Editor{

    // QemuSensorItem* sensor;
    QemuSensorModel& model;
    QScrollArea* scrollArea;
    QWidget* contentWidget;
    QVBoxLayout* contentLayout;
    QVBoxLayout* mainLayout;

    // parameters:
    QLineEdit* priority;
    QLineEdit* name;
    QComboBox* platform;
    QComboBox* machine;
    QComboBox* cpu;
    QSpinBox* memory_MB;
    QPushButton* kernal;
    QPushButton* harddrive;
    QPushButton* cdrom;
    QComboBox* boot;
    QLineEdit* net;
    QLineEdit* append;
    QCheckBox* nographic;

    QString m_kernal;
    QString m_harddrive;
    QString m_cdrom;

    // inits:
    void initPriority();
    void initName();
    void initPlatform();
    void initMachine();
    void initCpu();
    void initMemory_MB();
    void initKernal();
    void initHarddrive();
    void initCdrom();
    void initBoot();
    void initNet();
    void initAppend();
    void initNographic();

    void initSaveCancelBtns();
    void initParameters();
    void initUI();

    void onSaveBtnClicked();
    void onCancelBtnCliked();

public:
    Editor(QemuSensorModel* _model);
    void open();
    void close();
};


#endif // QEMUSENSORITEM_EDITOR_H
