#include "sensorItem.h"
#include <QPainter>
// #include <QIcon>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include <cmath>
#include "gui/popupdialog.h"

    SensorItem::SensorItem( QGraphicsItem *parent): BaseItem(parent){
        m_type = ItemType::Sensor;
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
    {
        priority = other.priority;
        name = other.name;
        buildCommand = other.buildCommand;
        runCommand = other.runCommand;
    }

    void SensorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        painter->setBrush(Qt::transparent);
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

QString SensorItem::getBuildCommand() const {
    return buildCommand;
}

QString SensorItem::getRunCommand() const {
    return runCommand;
}

void SensorItem::setPriority(const QString& priority) {
    this->priority = priority;
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

bool SensorItem::isInitialized() const
{
    return !priority.isEmpty() && !name.isEmpty() && !buildCommand.isEmpty() && !runCommand.isEmpty();
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
            buildCommand = tempSensorItem->getBuildCommand();
            runCommand = tempSensorItem->getRunCommand();
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