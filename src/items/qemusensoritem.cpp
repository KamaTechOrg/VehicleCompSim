#include "qemusensoritem.h"
#include "qfiledialog.h"
#include "qfileselector.h"
#include "qpushbutton.h"
#include "./editors/QemuSensorItem_Editor.h"


void QemuSensorItem::updateItem()
{
    EditPanel::loadNewEditor(new QemuSensorItem::Editor(this));

}
