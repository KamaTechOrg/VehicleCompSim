#include "sensoritem.h"
#include <QPainter>
#include <QCheckBox>
#include <QMessageBox>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include "globalstate.h"
#include "CMakeUtils/getBuildAndRunCommands.h"
#include "editors/SensorItem_Editor.h"
#include "globalconstants.h"

#include <qrandom.h>
using namespace globalConstants;

SensorItem::SensorItem(SensorModel* model, QGraphicsItem *parent)
    : BaseItem(model, parent), m_model(model),
      m_checkBoxProxy(new QGraphicsProxyWidget(this)),
      m_verticalIndicatorProxy(new QGraphicsProxyWidget(this)),
      m_verticalIndicator(new VerticalIndicator()),
      m_globalState(GlobalState::getInstance())
{
    m_width = 160;
    m_height = 90;

    m_isOwnedByMe = m_model->ownerID() == m_globalState.myClientId();

    m_closeProxy->setPos(boundingRect().topRight() + QPointF(5, -25)); // Adjust position to be outside top-right
    setupCheckBoxProxy();
    // Set up vertical indicator
    m_verticalIndicator->setMaxValue(200); // Set max value
    m_verticalIndicator->setValue(50);
    m_verticalIndicatorProxy->setWidget(m_verticalIndicator);
    m_verticalIndicatorProxy->setPos(QPointF(boundingRect().right(), boundingRect().top() + 5));
    m_verticalIndicatorProxy->setZValue(1);
    
    connect(m_model, &SensorModel::anyPropertyChanged, this, &SensorItem::onModelUpdated);
    updateColor();
    hideButtons();

    // Set up timer for live updates
    m_updateWindowTimer = new QTimer(this);
    connect(m_updateWindowTimer, &QTimer::timeout, this, &SensorItem::showInfoWindow);
    // connect(&m_globalState, &GlobalState::parsedData, this, &SensorItem::update_new_data);

    setZValue(1);

}
SensorItem::~SensorItem() {
    delete m_persistentTooltip;
}
VerticalIndicator *SensorItem::getVerticalIndicator() const
{
  return m_verticalIndicator;
}

  // todo update sensor data
void SensorItem::update_new_data(QList<QPair<QString, QString>> data){
    auto *sensor = dynamic_cast<SensorItem *>(this);
    if(sensor->getModel().priority() == data[bufferInfo::SourceId].second || sensor->getModel().priority() == data[bufferInfo::DestinationId].second){
        all_data_final.emplace_back(data);
        last_data_final = data;
        qInfo() << "sensor need update";
    }
}
void SensorItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    QPointF nearestPoint;
    if (isNearConnectionPoint(event->pos(), &nearestPoint)) {
        m_hoveredPoint = nearestPoint;
        update(); // Trigger a repaint
    }
    if(m_globalState.isRunning() && !mouse_pressed){
        qInfo() << "tooltip";
        QString tooltipHtml = "<table border='1' cellspacing='0' cellpadding='3' style='border-collapse: collapse;'><tr>";
        for (const auto &item: last_data_final) {
            tooltipHtml += QString("<th>%1</th>").arg(item.first);
        }
        tooltipHtml += "</tr><tr>";
        for (const auto &item: last_data_final) {
            tooltipHtml += QString("<td>%1</td>").arg(item.second);
        }
        tooltipHtml += "</tr></table>";
        if (m_persistentTooltip == nullptr) {
            m_persistentTooltip = new PersistentTooltip();
        }
        m_persistentTooltip->setText(tooltipHtml);
        m_persistentTooltip->move(event->screenPos());
        m_persistentTooltip->show();
    }
    BaseItem::hoverEnterEvent(event);
}

void SensorItem::setupCheckBoxProxy()
{
    if (m_checkBoxProxy->widget() != nullptr) {
        // just update the checkbox state
        QCheckBox* checkBox = static_cast<QCheckBox*>(m_checkBoxProxy->widget());
        bool oldState = checkBox->blockSignals(true); // Block signals
        checkBox->setCheckState(m_model->isExcludeFromProject() ? Qt::Checked : Qt::Unchecked);
        checkBox->setEnabled(isInitialized() && m_isOwnedByMe);
        checkBox->blockSignals(oldState); // Restore previous state
        return;
    }

    // Create checkbox for excluding from project
    QCheckBox* excludeCheckBox = new QCheckBox("Exclude");
    excludeCheckBox->setToolTip("Exclude this sensor from the project");
    excludeCheckBox->setCheckState(m_model->isExcludeFromProject() ? Qt::Checked : Qt::Unchecked);
    excludeCheckBox->setAttribute(Qt::WA_TranslucentBackground);
    excludeCheckBox->setEnabled(isInitialized() && m_isOwnedByMe);
    connect(excludeCheckBox, &QCheckBox::stateChanged, [this](int state) {
        m_model->setisExcludeFromProject(state == Qt::Checked);
        m_model->notifyItemModified();
        updateColor();
    });

    m_checkBoxProxy->setWidget(excludeCheckBox);
    m_checkBoxProxy->setPos(QPointF(-m_width / 2 + 10, m_height / 2 - 30));
}

void SensorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->drawRect(boundingRect());

    painter->setRenderHint(QPainter::Antialiasing);

    QColor colorToUse = m_color;
    if (!m_isOwnedByMe) {
        QColor backgroundColor = Qt::white;
        colorToUse.setRed((colorToUse.red() + backgroundColor.red()) / 2);
        colorToUse.setGreen((colorToUse.green() + backgroundColor.green()) / 2);
        colorToUse.setBlue((colorToUse.blue() + backgroundColor.blue()) / 2);
    }

    painter->setBrush(colorToUse);
    painter->drawRoundedRect(QRectF(-m_width / 2, -m_height / 2, m_width, m_height), 10, 10);

    painter->setPen(Qt::black);
    painter->drawText(boundingRect().adjusted(10, 10, -10, -10), Qt::AlignLeft | Qt::AlignTop, "ID: " + m_model->priority());
    painter->drawText(boundingRect().adjusted(10, 30, -10, -10), Qt::AlignLeft | Qt::AlignTop, "Name: " + m_model->name());

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
bool SensorItem::isInitialized() const
{
    return !m_model->priority().isEmpty() && !m_model->name().isEmpty() && !m_model->buildCommand().isEmpty() && !m_model->runCommand().isEmpty();
}

bool SensorItem::isExludeFromProject() const
{
    return m_model->isExcludeFromProject();
}

SensorModel &SensorItem::getModel() const
{
    return *m_model;
}

void SensorItem::confirmRemove()
{
    if(!m_isOwnedByMe){
        QMessageBox::warning(nullptr, "Error", "Only the owner can remove the sensor");
        return;
    }
    BaseItem::confirmRemove();
}

void SensorItem::showButtons()
{
    m_closeProxy->setVisible(true);
}

void SensorItem::hideButtons()
{
    m_closeProxy->setVisible(false);
}

void SensorItem::onModelUpdated()
{
    if(m_model->x() != x() || m_model->y() != y()){
        setPos(m_model->x(), m_model->y());
    }
    setupCheckBoxProxy();
    updateColor();
}

void SensorItem::updateColor()
{
    if (!isInitialized()) {
        m_color = m_disabledColor;
    } else if (isExludeFromProject()) {
        m_color = m_excludedColor;
    } else {
        m_color = m_availableColor;
    }

    update(); // Trigger a repaint
}

void SensorItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mouse_pressed = true;
        m_globalState.setCurrentSensorModel(this->m_model);
        if(m_globalState.isRunning()){
            showInfoWindow();
        }
    }
    BaseItem::mousePressEvent(event);
}
void SensorItem::showInfoWindow() {
    if (!scene()) return;
    m_updateWindowTimer->start(1000);  // Update every second
    if (m_infoWindowProxy == nullptr) {
        m_infoWindow = new CustomInfoWindow();
        m_infoWindowProxy = new QGraphicsProxyWidget(this);
        m_infoWindowProxy = scene()->addWidget(m_infoWindow);
        m_infoWindowProxy->setZValue(1);
        connect(m_infoWindow, &CustomInfoWindow::closed, this, &SensorItem::onCustomWindowClosed);
    }

    updateInfoWindow();
    QPointF pos = mapToScene(boundingRect().topRight() + QPointF(10, 0));
    m_infoWindowProxy->setPos(pos);
    m_infoWindowProxy->show();
//    m_updateTimer->start(1000);  // Update every second

}
void SensorItem::updateInfoWindow() {
    if (!m_infoWindow) return;
    QString info = fetchDataInTable();
    m_infoWindow->setInfo(info);
}
QString SensorItem::fetchDataInTable() {
    QString windowInfoHtml = "<table border='1' cellspacing='0' cellpadding='3' style='border-collapse: collapse;'>";

    // Iterate through each list (table)
    for (const auto &list : all_data_final) {
        // Add header row
        windowInfoHtml += "<tr>";
        for (const auto &pair : list) {
            windowInfoHtml += QString("<th>%1</th>").arg(pair.first);
        }
        windowInfoHtml += "</tr>";

        // Add content row
        windowInfoHtml += "<tr>";
        for (const auto &pair : list) {
            windowInfoHtml += QString("<td>%1</td>").arg(pair.second);
        }
        windowInfoHtml += "</tr>";
    }

    windowInfoHtml += "</table>";

    return windowInfoHtml;
}

void SensorItem::onCustomWindowClosed() {
    mouse_pressed = false;
    m_updateWindowTimer->stop();
}
void SensorItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    if (m_persistentTooltip) {
        m_persistentTooltip->hide();
    }
    BaseItem::hoverLeaveEvent(event);
}



