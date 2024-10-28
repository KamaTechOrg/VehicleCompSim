#ifndef MAINCOMPUTERMODEL_H
#define MAINCOMPUTERMODEL_H

#include "sensormodel.h"
#include "../../MainComputer/include/MainComputer.h"
#include "../../MainComputer/include/JsonLoader.h"

class MainComputerModel : public SensorModel
{
    Q_OBJECT

    Q_PROPERTY(QString guiData READ guiData WRITE setGuiData NOTIFY guiDataChanged)
    Q_PROPERTY(QString logicData READ logicData WRITE setLogicData NOTIFY logicDataChanged)
public:
    MainComputerModel();
    ~MainComputerModel();

    void setGuiData(QString _guiData);
    QString guiData() const;

    void setLogicData(QString _logicData);
    QString logicData() const;

    // Override SerializableItem methods
    QJsonObject serialize() const override;
    void deserialize(const QJsonObject &itemData) override;

    MainComputer& computer();
signals:
    void guiDataChanged();
    void logicDataChanged();
    void modelExistChanged(bool exist);
private:

    MainComputer m_mainComputer;

};

#endif // MAINCOMPUTERMODEL_H
