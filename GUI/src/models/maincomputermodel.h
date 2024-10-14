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
    QString guiData();

    void setLogicData(QString _logicData);
    QString logicData();

    // Override SerializableItem methods
    QJsonObject serialize() const override;
    void deserialize(const QJsonObject &itemData) override;

    MainComputer& computer();
signals:
    void guiDataChanged();
    void logicDataChanged();

private:

    MainComputer m_mainComputer;
    std::string& guiDataRef = JsonLoader::guiData;
    std::string& logicDataRef = JsonLoader::logicData;

    static int objectsCount;
};

#endif // MAINCOMPUTERMODEL_H
