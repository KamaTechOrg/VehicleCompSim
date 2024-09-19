#include "QemuSensorItem_Editor.h"
#include "qlabel.h"
#include <QComboBox>
#include <QSpinBox>
#include <QSplitter>
#include "qfiledialog.h"
#include <GlobalState.h>
#include <climits>


void QemuSensorItem::Editor::initPriority()
{
    priority->setText(model.priority());
    layout->addWidget(new QLabel("priority", this));
    layout->addWidget(priority);
}

void QemuSensorItem::Editor::initName()
{
    name->setText(model.name());
    layout->addWidget(new QLabel("name", this));
    layout->addWidget(name);
}

void QemuSensorItem::Editor::initPlatform()
{
    for (auto &option: model.getPlatformOptions())
    {
        platform->addItem(option.label, option.value);
        if (model.platform() == option.value) platform->setCurrentText(option.label);
    }
    layout->addWidget(new QLabel("platform:", this));
    layout->addWidget(platform);
}

void QemuSensorItem::Editor::initMachine()
{
    for (auto option : model.getMachineOptions())
    {
        machine->addItem(option.label, option.value);
        if (model.machine().compare(option.value) == 0) machine->setCurrentText(option.label);
    }
    layout->addWidget(new QLabel("machine:", this));
    layout->addWidget(machine);
}

void QemuSensorItem::Editor::initCpu()
{
    for (auto option : model.getCpuOptions())
    {
        cpu->addItem(option.label, option.value);
        if (model.cpu().compare(option.value) == 0) cpu->setCurrentText(option.label);
    }

    layout->addWidget(new QLabel("cpu:", this));
    layout->addWidget(cpu);
}

void QemuSensorItem::Editor::initMemory_MB()
{
    memory_MB->setMinimum(1);
    memory_MB->setMaximum(INT_MAX);
    memory_MB->setValue(model.memory_MB());
    layout->addWidget(new QLabel("memory (in MB):", this));
    layout->addWidget(memory_MB);
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
    layout->addWidget(new QLabel("Kernal File:", this));
    layout->addWidget(kernal);
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
    layout->addWidget(new QLabel("Hard Drive File:", this));
    layout->addWidget(harddrive);
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
    layout->addWidget(new QLabel("cdrom File:", this));
    layout->addWidget(cdrom);
}

void QemuSensorItem::Editor::initBoot()
{
    for (auto option : model.getBootOptions())
    {
        boot->addItem(option.label, option.value);
        if (model.boot().compare(option.value) == 0) boot->setCurrentText(option.label);
    }

    layout->addWidget(new QLabel("start boot from device:", this));
    layout->addWidget(boot);
}

void QemuSensorItem::Editor::initNet()
{
    net->setText(model.net());
    layout->addWidget(new QLabel("network configuration (advanced)", this));
    layout->addWidget(net);
}

void QemuSensorItem::Editor::initAppend()
{
    append->setText(model.append());
    layout->addWidget(new QLabel("kernal startup parameters (advanced):", this));
    layout->addWidget(append);
}

void QemuSensorItem::Editor::initNographic()
{
    nographic->setCheckState((model.nographic() ? Qt::Checked : Qt::Unchecked));
    layout->addWidget(new QLabel("dont open a screen window:", this));
    layout->addWidget(nographic);
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
    layout->addLayout(row);
}

void QemuSensorItem::Editor::initLayout()
{
    setLayout(layout);
}
void QemuSensorItem::Editor::onSaveBtnClicked()
{
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

QemuSensorItem::Editor::Editor(QemuSensorModel *_model) : model(*_model), layout(new QVBoxLayout(this))
{
    initSaveCancelBtns();
    initParameters();
    initLayout();
}

void QemuSensorItem::Editor::open()
{
    this->show();
}

void QemuSensorItem::Editor::close()
{
    qInfo() << "closing qemu snesor" ;
}

