#ifndef QEMUSENSORITEM_EDITOR_H
#define QEMUSENSORITEM_EDITOR_H
#include "../qemusensoritem.h"
#include "../gui/editpanel.h"
#include <QVBoxLayout>



class QemuSensorItem::Editor : public EditPanel::Editor{

    QemuSensorItem* sensor;
    QVBoxLayout* layout;
public:
    Editor(QemuSensorItem* _sensor);
    void open();
    void close();
};


#endif // QEMUSENSORITEM_EDITOR_H
