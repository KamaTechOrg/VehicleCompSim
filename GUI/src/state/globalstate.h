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
    Q_PROPERTY(QString myClientId READ myClientId WRITE setMyClientId)

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
    void addProject(ProjectModel* project);

    ProjectModel* currentProject() const { return m_currentProject; }
    void setCurrentProject(ProjectModel* project);
    void publishCurrentProject();

    SensorModel* currentSensorModel() const { return m_currentSensorModel; }
    void setCurrentSensorModel(SensorModel* sensorModel);

    QString myClientId() const { return m_myClientId; }
    void setMyClientId(QString value);

//    QList<QList<QVariant>*> snesors_data() const { return snesors_log_data.values(); }
    void updateLogData(QString sensorId, QList<QVariant> data);
    void updateColumnNames(QString sensorId, QList<QString> data);
    void updateDbHandler(wint_t sensorId, QList<QList<QString>> data);

    void saveData();
    void loadData();


signals:
    void isOnlineChanged(bool isOnline);
    void isRemoteModeChanged(bool isRemoteMode);
    void isConnectingChanged(bool isConnecting);
    void isRunningChanged(bool isRunning);
    void currentProjectChanged(ProjectModel* project);
    void currentSensorModelChanged(SensorModel* sensorModel);
    void projectAdded(ProjectModel* project);
    void currentProjectPublished(ProjectModel* project);
    void dataLogAdded(QString sensorId, QList<QVariant> data);
    void ColumnNamesAdded(QString sensorId, QList<QString> data);
    void SensorDbInfoAdded(wint_t sensorId, QList<QList<QString>> data);
    void saveBtnPressed();
    void loadBtnPressed();



private:
    GlobalState(QObject* parent = nullptr);
    GlobalState(const GlobalState&) = delete;
    GlobalState& operator=(const GlobalState&) = delete;

    bool m_isOnline = false;
    bool m_isRemoteMode = false;
    bool m_isConnecting = false;
    bool m_isRunning = false;
    QString m_myClientId;
    QHash<QString, ProjectModel*> m_projects;
    ProjectModel* m_currentProject = nullptr;
    SensorModel* m_currentSensorModel = nullptr;


//    QHash<QString, QList<QVariant> *> snesors_log_data;

};
