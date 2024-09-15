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

    // setPos(m_model->x(), m_model->y());

    connect(m_model, &SensorModel::anyPropertyChanged, this, &SensorItem::onModelUpdated);
    
    updateColor();
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
    connect(updateButton, &QPushButton::clicked, this, &SensorItem::openEditor);
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

void SensorItem::openEditor() {
    //popupDialog->oldSensorItem = this;
   // popupDialog->reset();
    EditPanel::loadNewEditor(new Editor(this));
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
