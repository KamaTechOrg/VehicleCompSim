#include "globalstate.h"
#include <QSettings>

GlobalState& GlobalState::getInstance()
{
    static GlobalState instance;
    return instance;
}

void GlobalState::setIsOnline(bool value)
{
    if (m_isOnline != value) {
        m_isOnline = value;
        emit isOnlineChanged(m_isOnline);
    }
}

void GlobalState::setIsRemoteMode(bool value)
{
    if (m_isRemoteMode != value) {
        m_isRemoteMode = value;
        emit isRemoteModeChanged(m_isRemoteMode);
    }
}

void GlobalState::setIsConnecting(bool value)
{
    if (m_isConnecting != value) {
        m_isConnecting = value;
        emit isConnectingChanged(m_isConnecting);
    }
}

void GlobalState::setIsRunning(bool value)
{
    if (m_isRunning != value) {
        m_isRunning = value;
        emit isRunningChanged(m_isRunning);
    }
}

void GlobalState::addProject(ProjectModel* project)
{
    if (project) {
        QString projectId = project->id();
        if (!m_projects.contains(projectId)) {
            project->setParent(this);  
            m_projects.insert(projectId, project);
            emit projectAdded(project);
        }
    }
}

void GlobalState::setCurrentProject(ProjectModel* project)
{
    if (m_currentProject != project) {
        m_currentProject = project;
        emit currentProjectChanged(m_currentProject);
        QObject::connect(m_currentProject, &ProjectModel::modelRemoved , [this](SerializableItem* model){
            if (m_currentSensorModel == model) setCurrentSensorModel(nullptr);
        });
    }
}

void GlobalState::publishCurrentProject()
{
    if (m_currentProject) {
        m_currentProject->setPublished(true);
        emit currentProjectPublished(m_currentProject);
    }
}

void GlobalState::setCurrentSensorModel(SensorModel* sensorModel)
{
    if (m_currentSensorModel != sensorModel){
        m_currentSensorModel = sensorModel;
        emit currentSensorModelChanged(m_currentSensorModel);
    }
}

void GlobalState::setMyClientId(QString value)
{
    if (m_myClientId != value) {
        m_myClientId = value;
        QSettings settings("VehicleCompSim", "GUI");
        settings.setValue("clientId", m_myClientId);
    }
}

void GlobalState::updateLogData(QString sensorId, QList<QVariant> data){
    emit dataLogAdded(sensorId, data);
}
void GlobalState::updateColumnNames(QString sensorId, QList<QString> data){
    emit ColumnNamesAdded(sensorId, data);
}
void GlobalState::updateDbHandler(wint_t sensorId, QList<QList<QString>> data){
    emit SensorDbInfoAdded(sensorId, data);
}

GlobalState::GlobalState(QObject* parent) : QObject(parent) {
    QSettings settings("VehicleCompSim", "GUI");
    m_myClientId = settings.value("clientId").toString();
    m_myClientId = (m_myClientId.isNull() || m_myClientId.isEmpty()) ? "-1" : m_myClientId;
}


