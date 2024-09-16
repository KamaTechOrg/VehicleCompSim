#include "QemuSensorItem_Editor.h"
#include "qlabel.h"
#include <QComboBox>
#include <QSpinBox>
#include <QSplitter>
#include <climits>

QString getQemuOutputForParameter(const QString& flag);

QemuSensorItem::Editor::Editor(QemuSensorItem *_sensor) : sensor(_sensor), model(_sensor->getQemuModel()), layout(new QVBoxLayout(this))
{
    //parameters:

    // machine
    QComboBox* p_machine = new QComboBox(this);
    p_machine->addItem("[None]", "");

    auto opt_list = getQemuOutputForParameter("-machine").split('\n');
    opt_list.pop_front();
    for (auto opt: opt_list)
    {
        if (opt.isEmpty()) continue;
        int index = opt.indexOf(' ');
        auto value = opt.sliced(0, index).trimmed();
        auto text = opt.sliced(index).trimmed();
        p_machine->addItem(text, value);
        if (model.machine().compare(value) == 0) p_machine->setCurrentText(text);
    }
    QObject::connect(p_machine, &QComboBox::currentIndexChanged, [p_machine, this](){
        this->model.setMachine(p_machine->currentData().toString());
    });

    // cpu

    QComboBox* p_cpu = new QComboBox(this);
    p_cpu->addItem("[None]", "");
    opt_list = getQemuOutputForParameter("-cpu").split('\n');
    opt_list.pop_front();
    for (auto &opt: opt_list)
    {
        opt = opt.trimmed();
        if (opt.isEmpty()) continue;
        int index = opt.indexOf(' ');
        auto value = index > 0 ? opt.sliced(0, index) : opt;
        p_cpu->addItem(value, value);
        if (model.cpu().compare(value) == 0) p_cpu->setCurrentText(value);
    }
    QObject::connect(p_cpu, &QComboBox::currentIndexChanged, [p_cpu, this](){
        this->model.setCpu(p_cpu->currentData().toString());
    });

    // memory
    QSpinBox* p_memory_MB = new QSpinBox(this);
    p_memory_MB->setMinimum(1);
    p_memory_MB->setMaximum(INT_MAX);
    p_memory_MB->setValue(model.memory_MB());
    QObject::connect(p_memory_MB, &QSpinBox::valueChanged, [p_memory_MB, this](){
        model .setMemory_MB( p_memory_MB->value());
    });


    QString p_kernal;
    QString p_harddrive;
    QString p_cdrom;
    QString p_boot;
    QString p_net;
    QString p_append;
    bool p_nographic = true;

    layout->addWidget(new QLabel("machine:", this));
    layout->addWidget(p_machine);
    layout->addWidget(new QLabel("cpu:", this));
    layout->addWidget(p_cpu);
    layout->addWidget(new QLabel("memory (in MB0:", this));
    layout->addWidget(p_memory_MB);
    setLayout(layout);
}

void QemuSensorItem::Editor::open()
{
    this->show();
}

void QemuSensorItem::Editor::close()
{
    qInfo() << "closing qemu snesor" ;
}


#include <QProcess>
#include <QString>
#include <QDebug>

QString getQemuOutputForParameter(const QString& flag) {
    QProcess process;
    process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());

    process.start("qemu-system-aarch64" , {flag, "help"});
    process.waitForFinished();  // Wait for the command to finish executing

    QString output = process.readAllStandardOutput();  // Capture the standard output
    QString errorOutput = process.readAllStandardError();  // Capture any error output

    if (!errorOutput.isEmpty()) {
        qDebug() << "Error:" << errorOutput;
    }

    return output;  // Return the captured output as a string
}
