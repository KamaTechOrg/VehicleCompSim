#include "QemuSensorItem_Editor.h"
#include "qlabel.h"
#include <QComboBox>
#include <QMessageBox>
#include <QSpinBox>
#include <QSplitter>
#include "qfiledialog.h"
#include <GlobalState.h>
#include <climits>


void QemuSensorItem::Editor::initPriority()
{
    priority->setText(model.priority());
    contentLayout->addWidget(new QLabel("priority", this));
    contentLayout->addWidget(priority);
}

void QemuSensorItem::Editor::initName()
{
    name->setText(model.name());
    contentLayout->addWidget(new QLabel("name", this));
    contentLayout->addWidget(name);
}

void QemuSensorItem::Editor::initPlatform()
{
    for (auto &option: model.getPlatformOptions())
    {
        platform->addItem(option.label, option.value);
        if (model.platform() == option.value) platform->setCurrentText(option.label);
    }
    contentLayout->addWidget(new QLabel("platform:", this));
    contentLayout->addWidget(platform);
}

void QemuSensorItem::Editor::initMachine()
{
    for (auto option : model.getMachineOptions())
    {
        machine->addItem(option.label, option.value);
        if (model.machine().compare(option.value) == 0) machine->setCurrentText(option.label);
    }
    contentLayout->addWidget(new QLabel("machine:", this));
    contentLayout->addWidget(machine);
}

void QemuSensorItem::Editor::initCpu()
{
    for (auto option : model.getCpuOptions())
    {
        cpu->addItem(option.label, option.value);
        if (model.cpu().compare(option.value) == 0) cpu->setCurrentText(option.label);
    }

    contentLayout->addWidget(new QLabel("cpu:", this));
    contentLayout->addWidget(cpu);
}

void QemuSensorItem::Editor::initMemory_MB()
{
    memory_MB->setMinimum(1);
    memory_MB->setMaximum(INT_MAX);
    memory_MB->setValue(model.memory_MB());
    contentLayout->addWidget(new QLabel("memory (in MB):", this));
    contentLayout->addWidget(memory_MB);
}

void QemuSensorItem::Editor::initKernal()
{
    QObject::connect(kernal, &QPushButton::clicked, [this](){
        QString newFile = QFileDialog::getOpenFileName(this, "Select File",
                                                            m_kernal,
                                                            "all files (*.*)");
        if (!newFile.isEmpty()) m_kernal = newFile;
        kernal->setText(m_kernal);
    });
    kernal->setText(m_kernal);
    contentLayout->addWidget(new QLabel("Kernal File:", this));
    contentLayout->addWidget(kernal);
}

void QemuSensorItem::Editor::initHarddrive()
{
    QObject::connect(harddrive, &QPushButton::clicked, [this](){
        QString newFile = QFileDialog::getOpenFileName(this, "Select File",
                                                       m_harddrive,
                                                       "all files (*.*)");
        if (!newFile.isEmpty()) m_harddrive = newFile;
        harddrive->setText(m_harddrive);
    });
    harddrive->setText(m_harddrive);
    contentLayout->addWidget(new QLabel("Hard Drive File:", this));
    contentLayout->addWidget(harddrive);
}

void QemuSensorItem::Editor::initCdrom()
{
    QObject::connect(cdrom, &QPushButton::clicked, [this](){
        QString newFile = QFileDialog::getOpenFileName(this, "Select File",
                                                       m_cdrom,
                                                       "all files (*.*)");
        if (!newFile.isEmpty()) m_cdrom = newFile;
        cdrom->setText(m_cdrom);
    });
    cdrom->setText(m_cdrom);
    contentLayout->addWidget(new QLabel("cdrom File:", this));
    contentLayout->addWidget(cdrom);
}

void QemuSensorItem::Editor::initBoot()
{
    for (auto option : model.getBootOptions())
    {
        boot->addItem(option.label, option.value);
        if (model.boot().compare(option.value) == 0) boot->setCurrentText(option.label);
    }

    contentLayout->addWidget(new QLabel("start boot from device:", this));
    contentLayout->addWidget(boot);
}

void QemuSensorItem::Editor::initNet()
{
    net->setText(model.net());
    contentLayout->addWidget(new QLabel("network configuration (advanced)", this));
    contentLayout->addWidget(net);
}

void QemuSensorItem::Editor::initAppend()
{
    append->setText(model.append());
    contentLayout->addWidget(new QLabel("kernal startup parameters (advanced):", this));
    contentLayout->addWidget(append);
}

void QemuSensorItem::Editor::initNographic()
{
    nographic->setCheckState((model.nographic() ? Qt::Checked : Qt::Unchecked));
    contentLayout->addWidget(new QLabel("don't open a screen window:", this));
    contentLayout->addWidget(nographic);
}

void QemuSensorItem::Editor::initSaveCancelBtns()
{
    QHBoxLayout *row = new QHBoxLayout;
    QPushButton* save = new QPushButton("Save", this);
    QPushButton* cancel = new QPushButton("Cancel", this);

    QObject::connect(save, &QPushButton::clicked, this, &Editor::onSaveBtnClicked);
    QObject::connect(cancel, &QPushButton::clicked, this, &Editor::onCancelBtnCliked);

    row->addWidget(save);
    row->addWidget(cancel);
    mainLayout->addLayout(row);
}

void QemuSensorItem::Editor::onSaveBtnClicked()
{
    if(!(model.ownerID() == GlobalState::getInstance().myClientId()))
    {
        QMessageBox::warning(this, "Error", "Only the owner can modify the sensor");
        return;
    }
    model.setPriority(priority->text());
    model.setName(name->text());
    model.setPlatform(platform->currentData().toString());
    model.setMachine(machine->currentData().toString());
    model.setCpu(cpu->currentData().toString());
    model.setMemory_MB(memory_MB->value());
    model.setKernal(m_kernal);
    model.setHarddrive(m_harddrive);
    model.setCdrom(m_cdrom);
    model.setBoot(boot->currentData().toString());
    model.setNet(net->text());
    model.setAppend(append->text());
    model.setNographic(nographic->checkState() == Qt::Checked);

    model.notifyItemModified();
    onCancelBtnCliked();
}

void QemuSensorItem::Editor::onCancelBtnCliked()
{
    GlobalState::getInstance().setCurrentSensorModel(nullptr);
}

void QemuSensorItem::Editor::initParameters()
{
    initPriority();
    initName();
    initPlatform();
    initMachine();
    initCpu();
    initMemory_MB();
    initKernal();
    initHarddrive();
    initCdrom();
    initBoot();
    initNet();
    initAppend();
    initNographic();
}

QemuSensorItem::Editor::Editor(QemuSensorModel* _model)
    : model(*_model),
      scrollArea(new QScrollArea(this)),
      contentWidget(new QWidget()),
      contentLayout(new QVBoxLayout(contentWidget)),
      mainLayout(new QVBoxLayout(this))
{
    initUI();
    initParameters();
    initSaveCancelBtns();
    setLayout(mainLayout);
}

void QemuSensorItem::Editor::initUI()
{
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);

    priority = new QLineEdit(contentWidget);
    name = new QLineEdit(contentWidget);
    platform = new QComboBox(contentWidget);
    machine = new QComboBox(contentWidget);
    cpu = new QComboBox(contentWidget);
    memory_MB = new QSpinBox(contentWidget);
    kernal = new QPushButton(contentWidget);
    harddrive = new QPushButton(contentWidget);
    cdrom = new QPushButton(contentWidget);
    boot = new QComboBox(contentWidget);
    net = new QLineEdit(contentWidget);
    append = new QLineEdit(contentWidget);
    nographic = new QCheckBox(contentWidget);
}

void QemuSensorItem::Editor::open()
{
    this->show();
}

void QemuSensorItem::Editor::close()
{
    qInfo() << "closing qemu snesor" ;
    GlobalState::getInstance().log("closing qemu snesor", "Terminal");

}

