#include "sensoritem.h"
#include <QPainter>
#include <QCheckBox>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include "globalstate.h"
#include "CMakeUtils/getBuildAndRunCommands.h"
#include "editors/SensorItem_Editor.h"

SensorItem::SensorItem(SensorModel* model, QGraphicsItem *parent)
    : BaseItem(model, parent), m_model(model),
      m_updateProxy(new QGraphicsProxyWidget(this)),
      m_checkBoxProxy(new QGraphicsProxyWidget(this))
{
    m_width = 160;
    m_height = 90;

    m_closeProxy->setPos(boundingRect().topRight() + QPointF(5, -25)); // Adjust position to be outside top-right
    setupUpdateButtonProxy();
    setupCheckBoxProxy();
    connect(m_model, &SensorModel::anyPropertyChanged, this, &SensorItem::onModelUpdated);
    updateColor();
    hideButtons();

    // Set up timer for live updates
    m_updateWindowTimer = new QTimer(this);
    connect(m_updateWindowTimer, &QTimer::timeout, this, &SensorItem::showInfoWindow);
    connect(&GlobalState::getInstance(), &GlobalState::ColumnNamesAdded, this, &SensorItem::update_column_names);

}
SensorItem::~SensorItem() {
    delete m_persistentTooltip;
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
    connect(updateButton, &QPushButton::clicked, this, &SensorItem::select);

}

void SensorItem::setupCheckBoxProxy()
{
    if (m_checkBoxProxy->widget() != nullptr) {
        // just update the checkbox state
        QCheckBox* checkBox = static_cast<QCheckBox*>(m_checkBoxProxy->widget());
        bool oldState = checkBox->blockSignals(true); // Block signals
        checkBox->setCheckState(m_model->isExcludeFromProject() ? Qt::Checked : Qt::Unchecked);
        checkBox->setEnabled(isInitialized());
        checkBox->blockSignals(oldState); // Restore previous state
        return;
    }

    // Create checkbox for excluding from project
    QCheckBox* excludeCheckBox = new QCheckBox("Exclude");
    excludeCheckBox->setToolTip("Exclude this sensor from the project");
    excludeCheckBox->setCheckState(m_model->isExcludeFromProject() ? Qt::Checked : Qt::Unchecked);
    excludeCheckBox->setAttribute(Qt::WA_TranslucentBackground);
    excludeCheckBox->setEnabled(isInitialized());
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
    painter->setBrush(m_color);
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

SensorModel &SensorItem::getModel()
{
    return *m_model;
}

void SensorItem::select()
{
    GlobalState::getInstance().setCurrentSensorModel(this->m_model);
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

void SensorItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    QPointF nearestPoint;
    if (isNearConnectionPoint(event->pos(), &nearestPoint)) {
        m_hoveredPoint = nearestPoint;
        update(); // Trigger a repaint
    }
    if(!mouse_pressed){
        QString tooltipHtml = "<table border='1' cellspacing='0' cellpadding='3' style='border-collapse: collapse;'><tr>";
        for (const QString &name: columnNames) {
            tooltipHtml += QString("<th>%1</th>").arg(name);
        }
        tooltipHtml += "</tr><tr>";
        for (int i = 0; i < last_data.size(); ++i) {
            QVariant value = last_data.value(i);
            tooltipHtml += QString("<td>%1</td>").arg(value.toString());
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

void SensorItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
//        if (event->button() == Qt::LeftButton && playMode) {
        mouse_pressed = true;
        showInfoWindow();
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
        connect(&GlobalState::getInstance(), &GlobalState::dataLogAdded, this, &SensorItem::update_data);
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
    windowInfoHtml += "<tr>";
    for (const QString &name: columnNames) {
        windowInfoHtml += QString("<th>%1</th>").arg(name);
    }
    windowInfoHtml += "</tr>";
    for (int i = 0; i < all_data.size(); i += columnNames.size()) {
        windowInfoHtml += "<tr>";
        for (int j = 0; j < columnNames.size() && (i + j) < all_data.size(); ++j) {
            QVariant value = all_data.value(i + j);
            windowInfoHtml += QString("<td>%1</td>").arg(value.toString());
        }
        windowInfoHtml += "</tr>";
    }

    windowInfoHtml += "</table>";
    return windowInfoHtml;
}
void SensorItem::onCustomWindowClosed() {
    m_updateWindowTimer->stop();
    mouse_pressed = false;
}
void SensorItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    if (m_persistentTooltip) {
        m_persistentTooltip->hide();
    }
    BaseItem::hoverLeaveEvent(event);
}
void SensorItem::update_data(const QString& sensorId, QList<QVariant> data){
    auto *sensor = dynamic_cast<SensorItem *>(this);
    if(sensor->getModel().priority() == sensorId){
        last_data.clear();
        all_data.clear();
        all_data = data;
        if(!data.empty()) {
            for (int i = 0; i < columnNames.size(); i++) {
                last_data.emplace_back(data[i]);
            }
        }
    }
}
void SensorItem::update_column_names(const QString& sensorId, QList<QString> data){
    auto *sensor = dynamic_cast<SensorItem *>(this);
    if(sensor->getModel().priority() == sensorId){
        columnNames = data;
    }
}



