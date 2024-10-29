#include "qemusensormodel.h"
#include <QProcess>

QemuSensorModel::QemuSensorModel()
    : m_platform(getPlatformOptions()[0].value)
{
    m_type = ItemType::Qemu;
}

QString QemuSensorModel::platform() const
{
    return m_platform;
}

QString QemuSensorModel::machine() const
{
    return m_machine;
}

QString QemuSensorModel::cpu() const
{
    return m_cpu;
}

int QemuSensorModel::memory_MB() const
{
    return m_memory_MB;
}

QString QemuSensorModel::kernal() const
{
    return m_kernal;
}

QString QemuSensorModel::harddrive() const
{
    return m_harddrive;
}

QString QemuSensorModel::cdrom() const
{
    return m_cdrom;
}

QString QemuSensorModel::boot() const
{
    return m_boot;
}

QString QemuSensorModel::net() const
{
    return m_net;
}

QString QemuSensorModel::append() const
{
    return m_append;
}

bool QemuSensorModel::nographic() const
{
    return m_nographic;
}

void QemuSensorModel::setPlatform(const QString &_platform)
{
    if (m_platform != _platform)
    {
        m_platform = _platform;
        updateRunCommand();
        emit platformChanged();
    }
}

void QemuSensorModel::setMachine(const QString &_machine)
{
    if (m_machine != _machine)
    {
        m_machine = _machine;
        updateRunCommand();
        emit machineChanged();
    }
}

void QemuSensorModel::setCpu(const QString &_cpu)
{
    if (m_cpu != _cpu)
    {
        m_cpu = _cpu;
        updateRunCommand();
        emit cpuChanged();
    }
}

void QemuSensorModel::setMemory_MB(int _memory)
{
    if (m_memory_MB != _memory)
    {
        m_memory_MB = _memory;
        updateRunCommand();
        emit memory_MBChanged();
    }
}

void QemuSensorModel::setKernal(const QString &_kernal)
{
    if (m_kernal != _kernal)
    {
        m_kernal = _kernal;
        updateRunCommand();
        emit kernalChanged();
    }
}

void QemuSensorModel::setHarddrive(const QString &_harddrive)
{
    if (m_harddrive != _harddrive)
    {
        m_harddrive = _harddrive;        
        updateRunCommand();
        emit harddriveChanged();
    }
}

void QemuSensorModel::setCdrom(const QString &_cdrom)
{
    if (m_cdrom != _cdrom)
    {
        m_cdrom = _cdrom;
        updateRunCommand();
        emit cdromChanged();
    }
}

void QemuSensorModel::setBoot(const QString &_boot)
{
    if (m_boot != _boot)
    {
        m_boot = _boot;
        updateRunCommand();
        emit bootChanged();
    }
}

void QemuSensorModel::setNet(const QString &_net)
{
    if (m_net != _net)
    {
        m_net = _net;
        updateRunCommand();
        emit netChanged();
    }
}

void QemuSensorModel::setAppend(const QString &_append)
{
    if (m_append != _append)
    {
        m_append = _append;
        updateRunCommand();
        emit appendChanged();
    }
}

void QemuSensorModel::setNographic(bool _nographic)
{
    if (m_nographic != _nographic)
    {
        m_nographic = _nographic;
        updateRunCommand();
        emit nographicChanged();
    }
}

std::vector<parameter_option_t> QemuSensorModel::getPlatformOptions()
{
    return {
        {"Arm v8 (64bit)", "aarch64"},
        {"Arm v7 (32bit)", "arm"}
    };
}

std::vector<parameter_option_t> QemuSensorModel::getMachineOptions()
{
    std::vector<parameter_option_t> options;

    //options.push_back({"[None]", ""});

    auto opt_list = getQemuOutputForParameter(FLAG_MACHINE).split('\n');
    opt_list.pop_front();

    for (auto opt: opt_list)
    {
        opt = opt.trimmed();
        if (opt.isEmpty()) continue;
        int index = opt.indexOf(' ');
        auto value = opt.sliced(0, index).trimmed();
        auto text = opt.sliced(index).trimmed();
        options.push_back({text, value});
    }
    return options;
}

std::vector<parameter_option_t> QemuSensorModel::getCpuOptions()
{
    std::vector<parameter_option_t> options;

    options.push_back({"[None]", ""});

    auto opt_list = getQemuOutputForParameter(FLAG_CPU).split('\n');
    opt_list.pop_front();
    for (auto &opt: opt_list)
    {
        opt = opt.trimmed();
        if (opt.isEmpty()) continue;
        int index = opt.indexOf(' ');
        auto value = index > 0 ? opt.sliced(0, index) : opt;
        options.push_back({value, value});
    }
    return options;
}

std::vector<parameter_option_t> QemuSensorModel::getBootOptions()
{
    return {
        {"[None]", ""},
        {"Hard drive", "c"},
        {"CD rom", "d"},
    };
}


// Override SerializableItem methods
QJsonObject QemuSensorModel::serialize() const {
    QJsonObject itemData = SensorModel::serialize();
     itemData["platform"] = m_platform;
     itemData["machine"] = m_machine;
     itemData["cpu"] = m_cpu;
     itemData["memory_MB"] = m_memory_MB;
     itemData["kernal"] = m_kernal;
     itemData["harddrive"] = m_harddrive;
     itemData["cdrom"] = m_cdrom;
     itemData["boot"] = m_boot;
     itemData["net"] = m_net;
     itemData["append"] = m_append;
     itemData["nographic "] = m_nographic ;

     return itemData;
}

void QemuSensorModel::deserialize(const QJsonObject &itemData) {
    SensorModel::deserialize(itemData);

    m_platform = itemData["platform"].toString();
    m_machine = itemData["machine"].toString();
    m_cpu = itemData["cpu"].toString();
    m_memory_MB = itemData["memory_MB"].toInt();
    m_kernal = itemData["kernal"].toString();
    m_harddrive = itemData["harddrive"].toString();
    m_cdrom = itemData["cdrom"].toString();
    m_boot = itemData["boot"].toString();
    m_net = itemData["net"].toString();
    m_append = itemData["append"] .toString();
    m_nographic  = itemData["nographic "].isBool();

    emit anyPropertyChanged();
}

QString QemuSensorModel::getQemuOutputForParameter(const QString &flag)
{
        QProcess process;
        process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());

        process.start(QString("qemu-system-") + m_platform , {flag, "help"});
        process.waitForFinished();  // Wait for the command to finish executing

        QString output = process.readAllStandardOutput();  // Capture the standard output
        QString errorOutput = process.readAllStandardError();  // Capture any error output

        if (!errorOutput.isEmpty()) {
            qDebug() << "Error:" << errorOutput;
        }

        return output;  // Return the captured output as a string
}

void QemuSensorModel::updateRunCommand()
{
    QStringList args;
    args << (QString("qemu-system-") + m_platform);
    if (!m_machine.isEmpty())
    {
        args << FLAG_MACHINE << m_machine;
    }
    if (!m_cpu.isEmpty())
    {
        args << FLAG_CPU << m_cpu;
    }
    if (m_memory_MB)
    {
        args << FLAG_MEMORY_MB << QString::number(m_memory_MB);
    }
    if (!m_kernal.isEmpty())
    {
        args << FLAG_KERNAL << m_kernal;
    }
    if (!m_harddrive.isEmpty())
    {
        args << FLAG_HARDDRIVE << m_harddrive;
    }
    if (!m_cdrom.isEmpty())
    {
        args << FLAG_CDROM << m_cdrom;
    }
    if (!m_boot.isEmpty())
    {
        args << FLAG_BOOT << m_boot;
    }
    if (!m_net.isEmpty())
    {
        args << FLAG_NET << m_net;
    }
    if (!m_append.isEmpty())
    {
        args << FLAG_APPEND << m_append;
    }
    if (m_nographic)
    {
        args << FLAG_NOGRAPHIC;
    }

    setRunCommand(args.join(' '));
}

const char* QemuSensorModel::FLAG_MACHINE = "-machine";
const char* QemuSensorModel::FLAG_CPU = "-cpu";
const char* QemuSensorModel::FLAG_MEMORY_MB = "-m";
const char* QemuSensorModel::FLAG_KERNAL = "-kernal";
const char* QemuSensorModel::FLAG_HARDDRIVE = "";
const char* QemuSensorModel::FLAG_CDROM = "";
const char* QemuSensorModel::FLAG_BOOT = "";
const char* QemuSensorModel::FLAG_NET = "";
const char* QemuSensorModel::FLAG_APPEND = "";
const char* QemuSensorModel::FLAG_NOGRAPHIC = "-nographic";
