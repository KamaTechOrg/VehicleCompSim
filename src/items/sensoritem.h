#pragma once

#include "BaseItem.h"

class SensorItem : public BaseItem {
public:
    SensorItem(QGraphicsItem* parent = nullptr);
    //copy constructor
    SensorItem(const SensorItem& other);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QString getID() const;
    QString getName() const;
    QString getBuildCommand() const;
    QString getRunCommand() const;
    QString getCmakePath() const;
    bool isUseCmakePath() const;

    void setID(const QString& id);
    void setName(const QString& name);
    void setBuildCommand(const QString& buildCommand);
    void setRunCommand(const QString& runCommand);
    void setCmakePath(const QString& path);
    void setUseCmakePath(bool use);
    bool isInitialized() const;
    bool isExludeFromProject() const;


private:
    QString id = "";
    QString name = "";
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
