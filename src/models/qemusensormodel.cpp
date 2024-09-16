#include "qemusensormodel.h"

QemuSensorModel::QemuSensorModel() {}

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
        emit platformChanged();
    }
}

void QemuSensorModel::setMachine(const QString &_machine)
{
    if (m_machine != _machine)
    {
        m_machine = _machine;
        emit machineChanged();
    }
}

void QemuSensorModel::setCpu(const QString &_cpu)
{
    if (m_cpu != _cpu)
    {
        m_cpu = _cpu;
        emit cpuChanged();
    }
}

void QemuSensorModel::setMemory_MB(int _memory)
{
    if (m_memory_MB != _memory)
    {
        m_memory_MB = _memory;
        emit memory_MBChanged();
    }
}

void QemuSensorModel::setKernal(const QString &_kernal)
{
    if (m_kernal != _kernal)
    {
        m_kernal = _kernal;
        emit kernalChanged();
    }
}

void QemuSensorModel::setHarddrive(const QString &_harddrive)
{
    if (m_harddrive != _harddrive)
    {
        m_harddrive = _harddrive;
        emit harddriveChanged();
    }
}

void QemuSensorModel::setCdrom(const QString &_cdrom)
{
    if (m_cdrom != _cdrom)
    {
        m_cdrom = _cdrom;
        emit cdromChanged();
    }
}

void QemuSensorModel::setBoot(const QString &_boot)
{
    if (m_boot != _boot)
    {
        m_boot = _boot;
        emit bootChanged();
    }
}

void QemuSensorModel::setNet(const QString &_net)
{
    if (m_net != _net)
    {
        m_net = _net;
        emit netChanged();
    }
}

void QemuSensorModel::setAppend(const QString &_append)
{
    if (m_append != _append)
    {
        m_append = _append;
        emit appendChanged();
    }
}

void QemuSensorModel::setNographic(bool _nographic)
{
    if (m_nographic != _nographic)
    {
        m_nographic = _nographic;
        emit nographicChanged();
    }
}


// Override SerializableItem methods
QJsonObject QemuSensorModel::serialize() const {
    QJsonObject itemData = SerializableItem::serialize();
    // itemData["priority"] = m_priority;
    // itemData["name"] = m_name;
    // itemData["ownerID"] = m_ownerID;
    // itemData["buildCommand"] = m_buildCommand;
    // itemData["runCommand"] = m_runCommand;
    // itemData["cmakePath"] = m_cmakePath;
    // itemData["isUseCmakePath"] = m_isUseCmakePath;
    // itemData["isExcludeFromProject"] = m_isExcludeFromProject;
    return itemData;
}

void QemuSensorModel::deserialize(const QJsonObject &itemData) {
    SerializableItem::deserialize(itemData);
    // m_priority = itemData["priority"].toString();
    // m_name = itemData["name"].toString();
    // m_ownerID = itemData["ownerID"].toString();
    // m_buildCommand = itemData["buildCommand"].toString();
    // m_runCommand = itemData["runCommand"].toString();
    // m_cmakePath = itemData["cmakePath"].toString();
    // m_isUseCmakePath = itemData["isUseCmakePath"].toBool();
    // m_isExcludeFromProject = itemData["isExcludeFromProject"].toBool();

    emit anyPropertyChanged();
}

