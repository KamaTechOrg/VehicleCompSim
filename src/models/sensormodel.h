#pragma once

#include "serializableitem.h"

class SensorModel : public SerializableItem {
    Q_OBJECT
    Q_PROPERTY(QString priority READ priority WRITE setPriority NOTIFY priorityChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString ownerID READ ownerID WRITE setOwnerID NOTIFY ownerIDChanged)
    Q_PROPERTY(QString buildCommand READ buildCommand WRITE setBuildCommand NOTIFY buildCommandChanged)
    Q_PROPERTY(QString runCommand READ runCommand WRITE setRunCommand NOTIFY runCommandChanged)
    Q_PROPERTY(QString cmakePath READ cmakePath WRITE setCmakePath NOTIFY cmakePathChanged)
    Q_PROPERTY(bool isUseCmakePath READ isUseCmakePath WRITE setisUseCmakePath NOTIFY isUseCmakePathChanged)
    Q_PROPERTY(bool isExcludeFromProject READ isExcludeFromProject WRITE setisExcludeFromProject NOTIFY isExcludeFromProjectChanged)

public:
    explicit SensorModel(QObject* parent = nullptr);

    QString priority() const;
    void setPriority(const QString& priority);

    QString name() const;
    void setName(const QString& name);

    QString ownerID() const;
    void setOwnerID(const QString& ownerID);

    QString buildCommand() const;
    void setBuildCommand(const QString& buildCommand);

    QString runCommand() const;
    void setRunCommand(const QString& runCommand);

    QString cmakePath() const;
    void setCmakePath(const QString& cmakePath);

    bool isUseCmakePath() const;
    void setisUseCmakePath(bool isUseCmakePath);

    bool isExcludeFromProject() const;
    void setisExcludeFromProject(bool isExcludeFromProject);

    // Override SerializableItem methods
    QJsonObject serialize() const override;
    void deserialize(const QJsonObject &itemData) override;

signals:
    void priorityChanged();
    void nameChanged();
    void ownerIDChanged();
    void buildCommandChanged();
    void runCommandChanged();
    void cmakePathChanged();
    void isUseCmakePathChanged();
    void isExcludeFromProjectChanged();
    void anyPropertyChanged();

private:
    QString m_priority;
    QString m_name;
    QString m_ownerID;
    QString m_buildCommand;
    QString m_runCommand;
    QString m_cmakePath;
    bool m_isUseCmakePath;
    bool m_isExcludeFromProject;
    qreal m_x;
    qreal m_y;
};