#include "customwidget.h"
#include "maincomputermodel.h"
#include "qemusensormodel.h"
#include "sensoritem.h"
#include <QPainter>
//#include <QMouseEvent>
#include <QApplication>

CustomWidget::CustomWidget(const WIDGET_TYPES type, QWidget* parent)
    : QWidget(parent), m_type(type) {
    setFixedSize(90, 55);
    setAcceptDrops(true);

    if (type == MAIN_COMPUTER_ITEM)
    {
        QObject::connect(&GlobalState::getInstance(), &GlobalState::currentProjectChanged, [this](){
            switchMainComputerWidgetVisability();
            static auto modelAddConnection = QObject::connect(GlobalState::getInstance().currentProject(), &ProjectModel::modelAdded, this, &CustomWidget::switchMainComputerWidgetVisability);
            static auto modelRemovedConnection = QObject::connect(GlobalState::getInstance().currentProject(), &ProjectModel::modelRemoved, this, &CustomWidget::switchMainComputerWidgetVisability);

            QObject::disconnect(modelAddConnection);
            QObject::disconnect(modelRemovedConnection);

            modelAddConnection = QObject::connect(GlobalState::getInstance().currentProject(), &ProjectModel::modelAdded, this, &CustomWidget::switchMainComputerWidgetVisability);
            modelRemovedConnection = QObject::connect(GlobalState::getInstance().currentProject(), &ProjectModel::modelRemoved, this, &CustomWidget::switchMainComputerWidgetVisability);
        });


    }
}

SerializableItem *CustomWidget::toSerializableItem(WIDGET_TYPES type)
{
    switch (type) {
    case REGULAR_SENSOR_ITEM:
        return new SensorModel;
    case QEMU_SENSOR_ITEM:
            return new QemuSensorModel;
    case MAIN_COMPUTER_ITEM:
        return new MainComputerModel;
    default:
        return nullptr;
    }
}

void CustomWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    switch (m_type) {
    case REGULAR_SENSOR_ITEM:
        painter.drawRoundedRect(QRectF(5, 5, 80, 45), 4, 4);
        break;
    case QEMU_SENSOR_ITEM:
        painter.drawRoundedRect(QRectF(5, 5, 80, 45), 4, 4);
        painter.drawText(5, 5, 80, 45, Qt::AlignCenter, "QEMU");
        break;
    case MAIN_COMPUTER_ITEM:
        painter.drawRoundedRect(QRectF(5, 5, 80, 45), 4, 4);
        painter.drawText(5, 5, 80, 45, Qt::AlignCenter, "Main\nComputer");
        break;
    case BUS_ITEM:
        painter.drawEllipse(12.5, 12.5, 25, 25);
        break;
    default:
        break;
    }
}

void CustomWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
    }
}

void CustomWidget::mouseMoveEvent(QMouseEvent* event) {
    if (currentProjectHasMainComputer) return;

    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    mimeData->setText(QString::number(m_type));
    drag->setMimeData(mimeData);

    QPixmap pixmap(size());
    pixmap.fill(Qt::transparent);
    render(&pixmap);
    drag->setPixmap(pixmap);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction);
}

void CustomWidget::switchMainComputerWidgetVisability()
{
    currentProjectHasMainComputer = false;
    for (auto model: GlobalState::getInstance().currentProject()->models())
    {
        if (dynamic_cast<MainComputerModel*>(model))
        {
            currentProjectHasMainComputer = true;
            break;
        }
    }
    setStyleSheet(QString() + "color: " + (currentProjectHasMainComputer ?  "red" : "green") + ";");
}
