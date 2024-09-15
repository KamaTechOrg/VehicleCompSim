#ifndef QEMUSENSORITEM_H
#define QEMUSENSORITEM_H
#include "sensoritem.h"
#include "../gui/editpanel.h"

class QemuSensorItem : public SensorItem
{
private:
   void updateItem() override;

   //parameters:
   QString p_platform;
   QString p_machine;
   QString p_cpu;
    int p_memory_MB = 512;
   QString p_kernal;
   QString p_harddrive;
   QString p_cdrom;
   QString p_boot;
   QString p_net;
   QString p_append;
   bool p_nographic = true;

public:
   class Editor;

};

#endif // QEMUSENSORITEM_H
