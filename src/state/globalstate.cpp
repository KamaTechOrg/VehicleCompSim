#include "globalstate.h"

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

void GlobalState::addProject(ProjectModel* project, bool local)
{
    if (project) {
        QString projectId = project->id();
        if (!m_projects.contains(projectId)) {
            project->setParent(this);  
            m_projects.insert(projectId, project);
            if (local) {
                emit projectAddedLocally(project);
            } else {
                emit projectAdded(project);
            }
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

void GlobalState::setCurrentSensorModel(SensorModel* sensorModel)
{
    if (m_currentSensorModel != sensorModel){
        m_currentSensorModel = sensorModel;
        emit currentSensorModelChanged(m_currentSensorModel);
    }
}

GlobalState::GlobalState(QObject* parent) : QObject(parent) {}
