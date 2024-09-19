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



class QemuSensorItem::Editor : public EditPanel::Editor{

    QemuSensorItem* sensor;
    QemuSensorModel& model;
    QVBoxLayout* layout;

    // parameters:
    QLineEdit* priority = new QLineEdit(this);
    QLineEdit* name = new QLineEdit(this);
    QComboBox* platform = new QComboBox(this);
    QComboBox* machine = new QComboBox(this);
    QComboBox* cpu = new QComboBox(this);
    QSpinBox* memory_MB = new QSpinBox(this);
    QPushButton* kernal = new QPushButton (this);
    QPushButton* harddrive = new QPushButton(this);
    QPushButton* cdrom = new QPushButton(this);
    QComboBox* boot = new QComboBox(this);
    QLineEdit* net = new QLineEdit(this);
    QLineEdit* append = new QLineEdit(this);
    QCheckBox* nographic = new QCheckBox(this);




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
    void initLayout();

    void onSaveBtnClicked();
    void onCancelBtnCliked();

public:
    Editor(QemuSensorModel* _model);
    void open();
    void close();
};


#endif // QEMUSENSORITEM_EDITOR_H
