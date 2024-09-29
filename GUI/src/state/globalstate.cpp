#include "globalstate.h"
#include <QSettings>

GlobalState& GlobalState::getInstance()
{
    static GlobalState instance;
    return instance;
}

void GlobalState::setIsRemoteMode(bool value)
{
    if (m_isRemoteMode != value) {
        m_isRemoteMode = value;
        emit isRemoteModeChanged(m_isRemoteMode);
    }
}

void GlobalState::setConnectionState(globalConstants::ConnectionState value)
{
    if (m_connectionState != value) {
        m_connectionState = value;
        emit connectionStateChanged(m_connectionState);
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

void GlobalState::setMaxMessageCount(int value)
{
    if (m_maxMessageCount != value) {
        m_maxMessageCount = value;
        emit maxMessageCountChanged(m_maxMessageCount);
    }
}

void GlobalState::newData(const QString& data){
    emit newDataArrived(data);
}
void GlobalState::ParserInfo(QMap<int, QList<QList<QString>>> parseInfoMap){
    emit ParserInfoArrived(parseInfoMap);
}
void GlobalState::newParsedData(QList<QPair<QString, QString>> data){
    emit parsedData(data);
}
void GlobalState::saveData(){
    emit saveBtnPressed();
}
void GlobalState::loadData(){
    emit loadBtnPressed();
}

GlobalState::GlobalState(QObject* parent) : QObject(parent) {
    QSettings settings("VehicleCompSim", "GUI");
    m_myClientId = settings.value("clientId").toString();
    m_myClientId = (m_myClientId.isNull() || m_myClientId.isEmpty()) ? "-1" : m_myClientId;
}

void GlobalState::new_test_buffer(const QString& data) {
    emit new_test_buffer_arrived(data);
}


