#pragma once

#include "baseitem.h"

class SensorItem : public BaseItem {
public:
    SensorItem(QGraphicsItem* parent = nullptr);
    //copy constructor
    SensorItem(const SensorItem& other);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QString getPriority() const;
    QString getName() const;
    QString getOwnerID() const;
    QString getBuildCommand() const;
    QString getRunCommand() const;
    QString getCmakePath() const;
    bool isUseCmakePath() const;

    void setPriority(const QString& priority);
    void setName(const QString& name);
    void setOwnerID(const QString& ownerID);
    void setBuildCommand(const QString& buildCommand);
    void setRunCommand(const QString& runCommand);
    void setCmakePath(const QString& path);
    void setUseCmakePath(bool use);
    bool isInitialized() const;
    bool isExludeFromProject() const;

    QJsonObject serialize() const override;
    void deserialize(const QJsonObject &itemData) override;

private:
    QString priority = "";
    QString name = "";
    QString ownerID = "";
    QString buildCommand = "";
    QString runCommand = "";
    QString cmakePath = "";
    bool useCmakePath = true;
    bool excludeFromProject = false;
    void updateItem();
    void showButtons();
    void hideButtons();

    QGraphicsProxyWidget* m_updateProxy;
};
