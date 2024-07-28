#pragma once

#include "BaseItem.h"

class SensorItem : public BaseItem {
public:
    SensorItem(QGraphicsItem* parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QString getID() const;
    QString getName() const;
    QString getBuildCommand() const;
    QString getRunCommand() const;

    void setID(const QString& id);
    void setName(const QString& name);
    void setBuildCommand(const QString& buildCommand);
    void setRunCommand(const QString& runCommand);

    bool isInitialized() const;

private:
    QString id = "";
    QString name = "";
    QString buildCommand = "";
    QString runCommand = "";
};
