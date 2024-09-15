#include "qemusensoritem.h"
#include "qfiledialog.h"
#include "qfileselector.h"
#include "qpushbutton.h"
#include "./editors/QemuSensorItem_Editor.h"


void QemuSensorItem::openEditor()
{
    EditPanel::loadNewEditor(new QemuSensorItem::Editor(this));

}

QString QemuSensorItem::getRunCommand() const
{
 //TODO
}
