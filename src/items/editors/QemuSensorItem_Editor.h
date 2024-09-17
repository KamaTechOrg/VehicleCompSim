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
    QComboBox* m_platform = new QComboBox(this);
    QComboBox* m_machine = new QComboBox(this);
    QComboBox* m_cpu = new QComboBox(this);
    QSpinBox* m_memory_MB = new QSpinBox(this);
    QPushButton* m_kernal = new QPushButton (this);
    QPushButton* m_harddrive = new QPushButton(this);
    QPushButton* m_cdrom = new QPushButton(this);
    QComboBox* m_boot = new QComboBox(this);
    QLineEdit* m_net = new QLineEdit(this);
    QLineEdit* m_append = new QLineEdit(this);
    QCheckBox* m_nographic = new QCheckBox(this);

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

    void initParameters();
    void initLayout();

public:
    Editor(QemuSensorItem* _sensor);
    void open();
    void close();
};


#endif // QEMUSENSORITEM_EDITOR_H
