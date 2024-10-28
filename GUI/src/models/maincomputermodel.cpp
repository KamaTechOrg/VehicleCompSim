#include "maincomputermodel.h"


MainComputerModel::MainComputerModel() {
    m_type = ItemType::MainComputer;
    setPriority(0);
    setName("Main Computer");
    setRunCommand("a private running method");
}

MainComputerModel::~MainComputerModel()
{
}

void MainComputerModel::setGuiData(QString _guiData)
{
    auto newJson = nlohmann::json::parse(_guiData.toStdString());

    if (newJson != m_mainComputer.guiData)
    {
        m_mainComputer.guiData = newJson;
        emit guiDataChanged();
    }
}

QString MainComputerModel::guiData() const
{
    return QString::fromStdString(m_mainComputer.guiData.dump());
}

void MainComputerModel::setLogicData(QString _logicData)
{
    auto newJson = nlohmann::json::parse(_logicData.toStdString());

    if (newJson != m_mainComputer.logicData)
    {
        m_mainComputer.logicData = newJson;
        emit logicDataChanged();
    }
}

QString MainComputerModel::logicData() const
{
    return QString::fromStdString(m_mainComputer.logicData.dump());
}


// Override SerializableItem methods
QJsonObject MainComputerModel::serialize() const {
    QJsonObject itemData = SensorModel::serialize();
    itemData["guiData"] = guiData();
    itemData["logicData"] = logicData();
    return itemData;
}

void MainComputerModel::deserialize(const QJsonObject &itemData) {
    SensorModel::deserialize(itemData);
    m_mainComputer.guiData = nlohmann::json::parse(
        itemData["guiData"].toString().toStdString());
    m_mainComputer.logicData = nlohmann::json::parse(
        itemData["logicData"].toString().toStdString());


    emit anyPropertyChanged();
}

MainComputer &MainComputerModel::computer()
{
    return m_mainComputer;
}
