#include "sensoritem.h"
#include <QPainter>
// #include <QIcon>
#include <QCheckBox>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include <cmath>
#include "gui/popupdialog.h"
#include "CMakeUtils/getBuildAndRunCommands.h"

SensorItem::SensorItem(QGraphicsItem *parent)
    : BaseItem(parent),
      m_updateProxy(new QGraphicsProxyWidget(this)),
      m_checkBoxProxy(new QGraphicsProxyWidget(this))
{
    m_type = ItemType::Sensor;
    m_width = 160;
    m_height = 90;

    m_closeProxy->setPos(boundingRect().topRight() + QPointF(5, -25)); // Adjust position to be outside top-right

    setupUpdateButtonProxy();
    setupCheckBoxProxy();

    hideButtons();
}

void SensorItem::setupUpdateButtonProxy()
{
    // Create update button
    QPushButton* updateButton = new QPushButton("↻");
    //updateButton->setIcon(QIcon(":/icons/update.png"));
    updateButton->setFixedSize(20, 20);
    updateButton->setToolTip("Update Item");

    m_updateProxy->setWidget(updateButton);
    m_updateProxy->setPos(boundingRect().topRight() + QPointF(30, -25)); // Position next to the close button

    // Connect button to its respective slot
    connect(updateButton, &QPushButton::clicked, this, &SensorItem::updateItem);
}

void SensorItem::setupCheckBoxProxy()
{
    // Create checkbox for excluding from project
    QCheckBox* excludeCheckBox = new QCheckBox("Exclude");
    excludeCheckBox->setToolTip("Exclude this sensor from the project");
    excludeCheckBox->setCheckState(excludeFromProject ? Qt::Checked : Qt::Unchecked);
    excludeCheckBox->setAttribute(Qt::WA_TranslucentBackground);
    connect(excludeCheckBox, &QCheckBox::stateChanged, [this](int state) {
        excludeFromProject = state == Qt::Checked;
        notifyItemModified();
    });

    m_checkBoxProxy->setWidget(excludeCheckBox);
    m_checkBoxProxy->setPos(QPointF(-m_width / 2 + 10, m_height / 2 - 30));
}

SensorItem::SensorItem(const SensorItem& other)
{
    priority = other.priority;
    name = other.name;
    buildCommand = other.buildCommand;
    runCommand = other.runCommand;
    cmakePath = other.cmakePath;
    useCmakePath = other.useCmakePath;
}

void SensorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//        painter->setBrush(Qt::green);
    painter->setPen(Qt::NoPen);
    painter->drawRect(boundingRect());

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(m_color);
    painter->drawRoundedRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), 10, 10);

    painter->setPen(Qt::black);
    painter->drawText(boundingRect().adjusted(10, 10, -10, -10), Qt::AlignLeft | Qt::AlignTop, "ID: " + priority);
    painter->drawText(boundingRect().adjusted(10, 30, -10, -10), Qt::AlignLeft | Qt::AlignTop, "Name: " + name);

    if (isSelected() || !m_hoveredPoint.isNull())
    {
        painter->setBrush(Qt::darkGray);
        for (const QPointF &point : connectionPoints())
        {
            if (isSelected() || point == m_hoveredPoint)
            {
                painter->drawEllipse(point, DotRadius, DotRadius);
            }
        }
    }
    if (isSelected()){
        showButtons();
    } else{
        hideButtons();
    }
}

QString SensorItem::getPriority() const {
    return priority;
}

QString SensorItem::getName() const {
    return name;
}

QString SensorItem::getOwnerID() const
{
    return ownerID;
}

QString SensorItem::getBuildCommand() const {
    return buildCommand;
}

QString SensorItem::getRunCommand() const {
    return runCommand;
}

void SensorItem::setPriority(const QString& priority) {
    this->priority = priority;
}

QString SensorItem::getCmakePath() const
{
    return cmakePath;
}

bool SensorItem::isUseCmakePath() const
{
    return useCmakePath;
}

void SensorItem::setName(const QString& name) {
    this->name = name;
}

void SensorItem::setOwnerID(const QString& ownerID)
{
    this->ownerID = ownerID;
}

void SensorItem::setBuildCommand(const QString& buildCommand) {
    this->buildCommand = buildCommand;
}

void SensorItem::setRunCommand(const QString& runCommand) {
    this->runCommand = runCommand;
}

void SensorItem::setCmakePath(const QString &path)
{
    cmakePath = path;
}

void SensorItem::setUseCmakePath(bool use)
{
    useCmakePath = use;
}

bool SensorItem::isInitialized() const
{
    return !priority.isEmpty() && !name.isEmpty() && !buildCommand.isEmpty() && !runCommand.isEmpty();
}

bool SensorItem::isExludeFromProject() const
{
    return excludeFromProject;
}

void SensorItem::updateItem()
{
    //copy the current item
    SensorItem* tempSensorItem = new SensorItem(*this);
    PopupDialog popup(tempSensorItem);
    popup.exec();
    if(popup.result() == QDialog::Accepted){
        if(tempSensorItem->isInitialized()){
            priority = tempSensorItem->getPriority();
            name = tempSensorItem->getName();
            ownerID = tempSensorItem->getOwnerID();
            buildCommand = tempSensorItem->getBuildCommand();
            runCommand = tempSensorItem->getRunCommand();
            cmakePath = tempSensorItem->getCmakePath();
            useCmakePath = tempSensorItem->isUseCmakePath();
            notifyItemModified();
        }
        else{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(nullptr, "Attention", 
                "Some fields are empty. Do you want to remove this item?",
                QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                removeItem();
            }
        }
    }
    delete tempSensorItem;
}

void SensorItem::showButtons()
{
    m_closeProxy->setVisible(true);
    m_updateProxy->setVisible(true);
}

void SensorItem::hideButtons()
{
    m_closeProxy->setVisible(false);
    m_updateProxy->setVisible(false);
}

QJsonObject SensorItem::serialize() const {
    QJsonObject itemData = BaseItem::serialize();
    itemData["priority"] = priority;
    itemData["name"] = name;
    itemData["ownerID"] = ownerID;
    itemData["buildCommand"] = buildCommand;
    itemData["runCommand"] = runCommand;
    itemData["cmakePath"] = cmakePath;
    itemData["useCmakePath"] = useCmakePath;
    itemData["excludeFromProject"] = excludeFromProject;
    return itemData;
}

void SensorItem::deserialize(const QJsonObject &itemData) {
    BaseItem::deserialize(itemData);
    priority = itemData["priority"].toString();
    name = itemData["name"].toString();
    ownerID = itemData["ownerID"].toString();
    buildCommand = itemData["buildCommand"].toString();
    runCommand = itemData["runCommand"].toString();
    cmakePath = itemData["cmakePath"].toString();
    useCmakePath = itemData["useCmakePath"].toBool();
    excludeFromProject = itemData["excludeFromProject"].toBool();
}