#pragma once

#include <QObject>
#include <QVector>
#include <QVariant>
#include "projectmodel.h"
#include "globalconstants.h"
#include "sensorModel.h"
#include "constants.h"

class GlobalState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isRemoteMode READ isRemoteMode WRITE setIsRemoteMode NOTIFY isRemoteModeChanged)
    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)
    Q_PROPERTY(globalConstants::ConnectionState connectionState READ connectionState WRITE setConnectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(ProjectModel* currentProject READ currentProject WRITE setCurrentProject NOTIFY currentProjectChanged)
    Q_PROPERTY(SensorModel* currentSensorModel READ currentSensorModel WRITE setCurrentSensorModel NOTIFY currentSensorModelChanged)
    Q_PROPERTY(QString myClientId READ myClientId WRITE setMyClientId)

public:
    static GlobalState& getInstance();

    bool isRemoteMode() const { return m_isRemoteMode; }
    void setIsRemoteMode(bool value);

    globalConstants::ConnectionState connectionState() const { return m_connectionState; }
    void setConnectionState(globalConstants::ConnectionState value);

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

    int maxMessageCount() const { return m_maxMessageCount; }
    void setMaxMessageCount(int value);

    void ParserInfo(QMap<int, QList<QList<QString>>> parseInfoMap);
    void newData(const QString buffer, size_t bufferSize);
    void newParsedData(QList<QPair<QString, QString>> data);

    void saveData();
    void loadData();

    // for test only
    void new_test_buffer(const char data[], size_t bufferSize);
//        void new_test_buffer(const QString& data);
    void setIsTest(bool value) { m_isTest = value; }
    bool isTest() const { return m_isTest; }



signals:
    void isRemoteModeChanged(bool isRemoteMode);
    void connectionStateChanged(globalConstants::ConnectionState connectionState);
    void isRunningChanged(bool isRunning);
    void currentProjectChanged(ProjectModel* project);
    void currentSensorModelChanged(SensorModel* sensorModel);
    void projectAdded(ProjectModel* project);
    void currentProjectPublished(ProjectModel* project);

    void maxMessageCountChanged(int maxMessageCount);
    void ParserInfoArrived(QMap<int, QList<QList<QString>>> parseInfoMap);
    void newDataArrived(const QString buffer, size_t bufferSize);
//    void newIIIIIIIArrived(const char buffer[], size_t bufferSize);
    void parsedData(QList<QPair<QString, QString>> data);
    void saveBtnPressed();
    void loadBtnPressed();

    // for test only
    void new_test_buffer_arrived(const char data[], size_t bufferSize);



private:
    GlobalState(QObject* parent = nullptr);
    GlobalState(const GlobalState&) = delete;
    GlobalState& operator=(const GlobalState&) = delete;

    bool m_isRemoteMode = false;
    bool m_isRunning = false;
    globalConstants::ConnectionState m_connectionState = globalConstants::ConnectionState::Offline;
    QString m_myClientId;
    QHash<QString, ProjectModel*> m_projects;
    ProjectModel* m_currentProject = nullptr;
    SensorModel* m_currentSensorModel = nullptr;

    bool m_isTest = false;
    int m_maxMessageCount = 100;
};
