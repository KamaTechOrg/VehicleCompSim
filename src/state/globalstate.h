#pragma once

#include <QObject>
#include <QVector>
#include "projectmodel.h"
#include "sensorModel.h"

class GlobalState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isOnline READ isOnline WRITE setIsOnline NOTIFY isOnlineChanged)
    Q_PROPERTY(bool isRemoteMode READ isRemoteMode WRITE setIsRemoteMode NOTIFY isRemoteModeChanged)
    Q_PROPERTY(bool isConnecting READ isConnecting WRITE setIsConnecting NOTIFY isConnectingChanged)
    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)
    Q_PROPERTY(ProjectModel* currentProject READ currentProject WRITE setCurrentProject NOTIFY currentProjectChanged)
    Q_PROPERTY(SensorModel* currentSensorModel READ currentSensorModel WRITE setCurrentSensorModel NOTIFY currentSensorModelChanged)

public:
    static GlobalState& getInstance();

    bool isOnline() const { return m_isOnline; }
    void setIsOnline(bool value);

    bool isRemoteMode() const { return m_isRemoteMode; }
    void setIsRemoteMode(bool value);

    bool isConnecting() const { return m_isConnecting; }
    void setIsConnecting(bool value);

    bool isRunning() const { return m_isRunning; }
    void setIsRunning(bool value);

    QList<ProjectModel*> projects() const { return m_projects.values(); }
    void addProject(ProjectModel* project, bool local = false);

    ProjectModel* currentProject() const { return m_currentProject; }
    void setCurrentProject(ProjectModel* project);

    SensorModel* currentSensorModel() const { return m_currentSensorModel; }
    void setCurrentSensorModel(SensorModel* sensorModel);

signals:
    void isOnlineChanged(bool isOnline);
    void isRemoteModeChanged(bool isRemoteMode);
    void isConnectingChanged(bool isConnecting);
    void isRunningChanged(bool isRunning);
    void currentProjectChanged(ProjectModel* project);
    void currentSensorModelChanged(SensorModel* sensorModel);
    void projectAdded(ProjectModel* project);
    void projectAddedLocally(ProjectModel* project);

private:
    GlobalState(QObject* parent = nullptr);
    GlobalState(const GlobalState&) = delete;
    GlobalState& operator=(const GlobalState&) = delete;

    bool m_isOnline = false;
    bool m_isRemoteMode = false;
    bool m_isConnecting = false;
    bool m_isRunning = false;
    QHash<QString, ProjectModel*> m_projects;
    ProjectModel* m_currentProject = nullptr;
    SensorModel* m_currentSensorModel = nullptr;

};
