#include "SensorModel.h"
#include "qmetaobject.h"

SensorModel::SensorModel(QObject* parent)
    : m_isUseCmakePath(true), m_isExcludeFromProject(false), m_x(0), m_y(0) {
    m_type = ItemType::Sensor;

}

QString SensorModel::priority() const { return m_priority; }
void SensorModel::setPriority(const QString& priority) {
    if (m_priority != priority) {
        m_priority = priority;
        emit priorityChanged();
    }
}

QString SensorModel::name() const { return m_name; }
void SensorModel::setName(const QString& name) {
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

QString SensorModel::ownerID() const { return m_ownerID; }
void SensorModel::setOwnerID(const QString& ownerID) {
    if (m_ownerID != ownerID) {
        m_ownerID = ownerID;
        emit ownerIDChanged();
    }
}

QString SensorModel::buildCommand() const { return m_buildCommand; }
void SensorModel::setBuildCommand(const QString& buildCommand) {
    if (m_buildCommand != buildCommand) {
        m_buildCommand = buildCommand;
        emit buildCommandChanged();
    }
}

QString SensorModel::runCommand() const { return m_runCommand; }
void SensorModel::setRunCommand(const QString& runCommand) {
    if (m_runCommand != runCommand) {
        m_runCommand = runCommand;
        emit runCommandChanged();
    }
}

QString SensorModel::cmakePath() const { return m_cmakePath; }
void SensorModel::setCmakePath(const QString& cmakePath) {
    if (m_cmakePath != cmakePath) {
        m_cmakePath = cmakePath;
        emit cmakePathChanged();
    }
}

bool SensorModel::isUseCmakePath() const { return m_isUseCmakePath; }
void SensorModel::setisUseCmakePath(bool isUseCmakePath) {
    if (m_isUseCmakePath != isUseCmakePath) {
        m_isUseCmakePath = isUseCmakePath;
        emit isUseCmakePathChanged();
    }
}

bool SensorModel::isExcludeFromProject() const { return m_isExcludeFromProject; }
void SensorModel::setisExcludeFromProject(bool isExcludeFromProject) {
    if (m_isExcludeFromProject != isExcludeFromProject) {
        m_isExcludeFromProject = isExcludeFromProject;
        emit isExcludeFromProjectChanged();
    }
}


// Override SerializableItem methods
QJsonObject SensorModel::serialize() const {
    QJsonObject itemData = SerializableItem::serialize();
    itemData["priority"] = m_priority;
    itemData["name"] = m_name;
    itemData["ownerID"] = m_ownerID;
    itemData["buildCommand"] = m_buildCommand;
    itemData["runCommand"] = m_runCommand;
    itemData["cmakePath"] = m_cmakePath;
    itemData["isUseCmakePath"] = m_isUseCmakePath;
    itemData["isExcludeFromProject"] = m_isExcludeFromProject;
    return itemData;
}

void SensorModel::deserialize(const QJsonObject &itemData) {
    SerializableItem::deserialize(itemData);
    m_priority = itemData["priority"].toString();
    m_name = itemData["name"].toString();
    m_ownerID = itemData["ownerID"].toString();
    m_buildCommand = itemData["buildCommand"].toString();
    m_runCommand = itemData["runCommand"].toString();
    m_cmakePath = itemData["cmakePath"].toString();
    m_isUseCmakePath = itemData["isUseCmakePath"].toBool();
    m_isExcludeFromProject = itemData["isExcludeFromProject"].toBool();

    emit anyPropertyChanged();
}

void SensorModel::autoConnectPropertySignals()
{
    const QMetaObject *metaObj = this->metaObject();
    for (int i = 0; i < metaObj->propertyCount(); ++i) {
        QMetaProperty property = metaObj->property(i);
        if (property.hasNotifySignal()) {
            // Get the notify signal for the property
            QMetaMethod notifySignal = property.notifySignal();

            // Connect the notify signal to anyPropertyChanged()
            connect(this, notifySignal, this, metaObj->method(metaObj->indexOfSignal("anyPropertyChanged()")));
        }
    }
}


