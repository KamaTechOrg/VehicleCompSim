#include "SensorItem.h"
#include <QPainter>
// #include <QIcon>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include <cmath>
#include "gui/popupdialog.h"
#include "../include/VehicleCompSim/utils/CMakeUtils/getBuildAndRunCommands.h"

    SensorItem::SensorItem( QGraphicsItem *parent):
    BaseItem(BaseItem::NodeType::Sensor, parent){
        m_width = 160;
        m_height = 90;

        m_closeProxy->setPos(boundingRect().topRight() + QPointF(5, -25)); // Adjust position to be outside top-right

        // Create update button
        QPushButton* updateButton = new QPushButton("↻");
        //updateButton->setIcon(QIcon(":/icons/update.png"));
        updateButton->setFixedSize(20, 20);
        updateButton->setToolTip("Update Item");

        QGraphicsProxyWidget* updateProxy = new QGraphicsProxyWidget(this);
        updateProxy->setWidget(updateButton);
        updateProxy->setPos(boundingRect().topRight() + QPointF(30, -25)); // Position next to the close button

        // Connect buttons to their respective slots
        connect(updateButton, &QPushButton::clicked, this, &SensorItem::updateItem);

        m_updateProxy = updateProxy;

        hideButtons();

    }

    SensorItem::SensorItem(const SensorItem& other)
        : BaseItem(BaseItem::NodeType::Sensor)
    {
        id = other.id;
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
        painter->drawText(boundingRect().adjusted(10, 10, -10, -10), Qt::AlignLeft | Qt::AlignTop, "ID: " + id);
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

QString SensorItem::getID() const {
    return id;
}

QString SensorItem::getName() const {
    return name;
}

QString SensorItem::getBuildCommand() const {
    return buildCommand;
}

QString SensorItem::getRunCommand() const {
    return runCommand;
}

QString SensorItem::getCmakePath() const
{
    return cmakePath;
}

bool SensorItem::isUseCmakePath() const
{
    return useCmakePath;
}

void SensorItem::setID(const QString& id) {
    this->id = id;
}

void SensorItem::setName(const QString& name) {
    this->name = name;
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
    return !id.isEmpty() && !name.isEmpty() && !buildCommand.isEmpty() && !runCommand.isEmpty();
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
            id = tempSensorItem->getID();
            name = tempSensorItem->getName();
            buildCommand = tempSensorItem->getBuildCommand();
            runCommand = tempSensorItem->getRunCommand();
            cmakePath = tempSensorItem->getCmakePath();
            useCmakePath = tempSensorItem->isUseCmakePath();
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
