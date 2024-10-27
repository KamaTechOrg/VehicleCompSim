#include "maincomputermodel.h"


MainComputerModel::MainComputerModel() {
    setPriority(0);
    setName("Main Computer");
    setRunCommand("a private running method");
}

MainComputerModel::~MainComputerModel()
{
}

void MainComputerModel::setGuiData(QString _guiData)
{
    if (_guiData != guiData())
   {
        JsonLoader::guiData = _guiData.toStdString();
        emit guiDataChanged();
    }
}

QString MainComputerModel::guiData()
{
    return QString::fromStdString(guiDataRef);
}

void MainComputerModel::setLogicData(QString _logicData)
{
    if (_logicData != logicData())
    {
        JsonLoader::logicData = _logicData.toStdString();
        emit logicDataChanged();
    }
}

QString MainComputerModel::logicData()
{
    return QString::fromStdString(logicDataRef);
}


// Override SerializableItem methods
QJsonObject MainComputerModel::serialize() const {
    QJsonObject itemData = SensorModel::serialize();
    itemData["guiData"] = guiDataRef.c_str();
    itemData["logicData"] = logicDataRef.c_str();
    return itemData;
}

void MainComputerModel::deserialize(const QJsonObject &itemData) {
    SensorModel::deserialize(itemData);
    guiDataRef = itemData["guiData"].toString().toStdString();
    logicDataRef = itemData["logicData"].toString().toStdString();


    emit anyPropertyChanged();
}

MainComputer &MainComputerModel::computer()
{
    return m_mainComputer;
}
