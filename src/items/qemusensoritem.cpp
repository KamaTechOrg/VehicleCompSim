#include "qemusensoritem.h"
#include "qfiledialog.h"
#include "qfileselector.h"
#include "qpushbutton.h"
#include "./editors/QemuSensorItem_Editor.h"
#include "models/qemusensormodel.h"

QemuSensorItem::QemuSensorItem() : SensorItem(new QemuSensorModel)
{

}

void QemuSensorItem::openEditor()
{
    EditPanel::loadNewEditor(new QemuSensorItem::Editor(this));

}

QString QemuSensorItem::getRunCommand() const
{
 //TODO
    return "";
}
