#ifndef SENSORITEM_EDITOR_H
#define SENSORITEM_EDITOR_H
#include "../sensoritem.h"

#include "gui/editpanel.h"
#include "qlineedit.h"
#include "qpushbutton.h"

class SensorItem::Editor : public EditPanel::Editor{
    SensorItem* sensor;
    QLineEdit* name;
    QPushButton* save;
public:
    Editor(SensorItem* _sensor);
    void open();
    void close();
};
#endif // SENSORITEM_EDITOR_H
