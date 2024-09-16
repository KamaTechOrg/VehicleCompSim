#ifndef QEMUSENSORITEM_H
#define QEMUSENSORITEM_H
#include "sensoritem.h"
#include "models/qemusensormodel.h"

class QemuSensorItem : public SensorItem
{
public:
    QemuSensorItem(QemuSensorModel* _model);
    QemuSensorModel& getQemuModel();
    class Editor;
    void openEditor() override;

private:
    QemuSensorModel* m_Qemu_model;
};

#endif // QEMUSENSORITEM_H
