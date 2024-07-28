#include "SensorItem.h"
#include <QPainter>
#include <cmath>

    SensorItem::SensorItem( QGraphicsItem *parent):
    BaseItem(BaseItem::NodeType::Sensor, parent){
        m_width = 160;
        m_height = 90;
    }

    void SensorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        painter->setBrush(Qt::transparent);
        painter->setPen(Qt::NoPen);
        painter->drawRect(boundingRect());

        painter->setRenderHint(QPainter::Antialiasing);
        painter->setBrush(m_color);
        painter->drawRoundedRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), 10, 10);

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

bool SensorItem::isInitialized() const
{
    return !id.isEmpty() && !name.isEmpty() && !buildCommand.isEmpty() && !runCommand.isEmpty();
}
