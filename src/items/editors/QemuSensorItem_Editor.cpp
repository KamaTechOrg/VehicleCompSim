#include "QemuSensorItem_Editor.h"
#include "qlabel.h"
#include <QComboBox>
#include <QSpinBox>
#include <QSplitter>
#include "qfiledialog.h"
#include <climits>


void QemuSensorItem::Editor::initPriority()
{
    priority->setText(model.priority());
    QObject::connect(priority, &QLineEdit::textChanged, [this](){
        model.setPriority(priority->text());
    });
    layout->addWidget(new QLabel("priority", this));
    layout->addWidget(priority);
}

void QemuSensorItem::Editor::initName()
{
    name->setText(model.name());
    QObject::connect(name,  &QLineEdit::textChanged  , [this](){
        model.setName(name->text());
    });
    layout->addWidget(new QLabel("name", this));
    layout->addWidget(name);
}

void QemuSensorItem::Editor::initPlatform()
{
    for (auto &option: model.getPlatformOptions())
    {
        m_platform->addItem(option.label, option.value);
        if (model.platform() == option.value) m_platform->setCurrentText(option.label);
    }
    QObject::connect(m_platform, &QComboBox::currentTextChanged, [this](){
        model.setPlatform(m_platform->currentData().toString());
    });
    layout->addWidget(new QLabel("platform:", this));
    layout->addWidget(m_platform);
}

void QemuSensorItem::Editor::initMachine()
{
    for (auto option : model.getMachineOptions())
    {
        m_machine->addItem(option.label, option.value);
        if (model.machine().compare(option.value) == 0) m_machine->setCurrentText(option.label);
    }
    QObject::connect(m_machine, &QComboBox::currentIndexChanged, [this](){
        this->model.setMachine(m_machine->currentData().toString());
    });
    layout->addWidget(new QLabel("machine:", this));
    layout->addWidget(m_machine);
}

void QemuSensorItem::Editor::initCpu()
{
    for (auto option : model.getCpuOptions())
    {
        m_cpu->addItem(option.label, option.value);
        if (model.cpu().compare(option.value) == 0) m_cpu->setCurrentText(option.label);
    }

    QObject::connect(m_cpu, &QComboBox::currentIndexChanged, [this](){
        this->model.setCpu(m_cpu->currentData().toString());
    });
    layout->addWidget(new QLabel("cpu:", this));
    layout->addWidget(m_cpu);
}

void QemuSensorItem::Editor::initMemory_MB()
{
    m_memory_MB->setMinimum(1);
    m_memory_MB->setMaximum(INT_MAX);
    m_memory_MB->setValue(model.memory_MB());
    QObject::connect(m_memory_MB, &QSpinBox::valueChanged, [this](){
        model .setMemory_MB( m_memory_MB->value());
    });
    layout->addWidget(new QLabel("memory (in MB):", this));
    layout->addWidget(m_memory_MB);
}

void QemuSensorItem::Editor::initKernal()
{
    QObject::connect(m_kernal, &QPushButton::clicked, [this](){
        QString newFile = QFileDialog::getOpenFileName(this, "Select File",
                                                            model.kernal(),
                                                            "all files (*.*)");
        if (!newFile.isEmpty()) model.setKernal(newFile);
        m_kernal->setText(model.kernal());
    });
    m_kernal->setText(model.kernal());
    layout->addWidget(new QLabel("Kernal File:", this));
    layout->addWidget(m_kernal);
}

void QemuSensorItem::Editor::initHarddrive()
{
    QObject::connect(m_harddrive, &QPushButton::clicked, [this](){
        QString newFile = QFileDialog::getOpenFileName(this, "Select File",
                                                       model.harddrive(),
                                                       "all files (*.*)");
        if (!newFile.isEmpty()) model.setHarddrive(newFile);
        m_harddrive->setText(model.harddrive());
    });
    m_harddrive->setText(model.harddrive());
    layout->addWidget(new QLabel("Hard Drive File:", this));
    layout->addWidget(m_harddrive);
}

void QemuSensorItem::Editor::initCdrom()
{
    QObject::connect(m_cdrom, &QPushButton::clicked, [this](){
        QString newFile = QFileDialog::getOpenFileName(this, "Select File",
                                                       model.cdrom(),
                                                       "all files (*.*)");
        if (!newFile.isEmpty()) model.setCdrom(newFile);
        m_cdrom->setText(model.cdrom());
    });
    m_cdrom->setText(model.cdrom());
    layout->addWidget(new QLabel("cdrom File:", this));
    layout->addWidget(m_cdrom);
}

void QemuSensorItem::Editor::initBoot()
{
    for (auto option : model.getBootOptions())
    {
        m_boot->addItem(option.label, option.value);
        if (model.boot().compare(option.value) == 0) m_boot->setCurrentText(option.label);
    }

    QObject::connect(m_boot, &QComboBox::currentIndexChanged, [this](){
        model.setBoot(m_boot->currentData().toString());
    });
    layout->addWidget(new QLabel("start boot from device:", this));
    layout->addWidget(m_boot);
}

void QemuSensorItem::Editor::initNet()
{
    m_net->setText(model.net());
    QObject::connect(m_net, &QLineEdit::textEdited, [this](){
        model.setNet(m_net->text());
    });
    layout->addWidget(new QLabel("network configuration (advanced)", this));
    layout->addWidget(m_net);
}

void QemuSensorItem::Editor::initAppend()
{
    m_append->setText(model.append());
    QObject::connect(m_append, &QLineEdit::textEdited, [this](){
        model.setAppend(m_append->text());
    });
    layout->addWidget(new QLabel("kernal startup parameters (advanced):", this));
    layout->addWidget(m_append);
}

void QemuSensorItem::Editor::initNographic()
{
    m_nographic->setCheckState((model.nographic() ? Qt::Checked : Qt::Unchecked));
    QObject::connect(m_nographic, &QCheckBox::checkStateChanged, [this](){
        model.setNographic(m_nographic->checkState() == Qt::Checked);
    });
    layout->addWidget(new QLabel("dont open a screen window:", this));
    layout->addWidget(m_nographic);
}

void QemuSensorItem::Editor::initLayout()
{
    setLayout(layout);
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

QemuSensorItem::Editor::Editor(QemuSensorItem *_sensor) : sensor(_sensor), model(_sensor->getQemuModel()), layout(new QVBoxLayout(this))
{
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

