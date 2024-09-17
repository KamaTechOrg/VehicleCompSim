#ifndef QEMUSENSORMODEL_H
#define QEMUSENSORMODEL_H
#include "sensormodel.h"
typedef struct {
    QString label;
    QString value;
} parameter_option_t;

class QemuSensorModel : public SensorModel
{
    Q_OBJECT

    Q_PROPERTY(QString platform READ platform WRITE setPlatform NOTIFY platformChanged)
    Q_PROPERTY(QString machine READ machine WRITE setMachine NOTIFY machineChanged)
    Q_PROPERTY(QString cpu READ cpu WRITE setCpu NOTIFY cpuChanged)
    //add cpu count
    Q_PROPERTY(int memory_MB READ memory_MB WRITE setMemory_MB NOTIFY memory_MBChanged)
    Q_PROPERTY(QString kernal READ kernal WRITE setKernal NOTIFY kernalChanged)
    Q_PROPERTY(QString harddrive READ harddrive WRITE setHarddrive NOTIFY harddriveChanged)
    Q_PROPERTY(QString cdrom READ cdrom WRITE setCdrom NOTIFY cdromChanged)
    Q_PROPERTY(QString boot READ boot WRITE setBoot NOTIFY bootChanged)
    Q_PROPERTY(QString net READ net WRITE setNet NOTIFY netChanged)
    Q_PROPERTY(QString net READ append WRITE setAppend NOTIFY appendChanged)
    Q_PROPERTY(bool nographic READ nographic WRITE setNographic NOTIFY nographicChanged)


public:
    QemuSensorModel();

    QString  platform() const;
    QString  machine() const;
    QString  cpu() const;
    int memory_MB() const;
    QString  kernal() const;
    QString  harddrive() const;
    QString  cdrom() const;
    QString  boot() const;
    QString  net() const;
    QString  append() const;
    bool nographic() const;

    void setPlatform(const QString& _platform );
    void setMachine(const QString& _machine);
    void setCpu(const QString & _cpu);
    void setMemory_MB(int _memory);
    void setKernal(const QString & _kernal);
    void setHarddrive(const QString & _harddrive);
    void setCdrom(const QString & _cdrom);
    void setBoot(const QString & _boot);
    void setNet(const QString & _net);
    void setAppend(const QString & _append);
    void setNographic(bool  _nographic);

    // paramaters options getters
    std::vector<parameter_option_t> getPlatformOptions();
    std::vector<parameter_option_t> getMachineOptions();
    std::vector<parameter_option_t> getCpuOptions();
    std::vector<parameter_option_t> getBootOptions();




    // Override SerializableItem methods
    QJsonObject serialize() const override;
    void deserialize(const QJsonObject &itemData) override;



signals:
    void platformChanged();
    void machineChanged();
    void cpuChanged();
    void memory_MBChanged();
    void kernalChanged();
    void harddriveChanged();
    void cdromChanged();
    void bootChanged();
    void netChanged();
    void appendChanged();
    void nographicChanged();
    void anyPropertyChanged();

private:

    //parameters:
    QString m_platform;
    QString m_machine;
    QString m_cpu;
    int m_memory_MB = 512;
    QString m_kernal;
    QString m_harddrive;
    QString m_cdrom;
    QString m_boot;
    QString m_net;
    QString m_append;
    bool m_nographic = true;

    // private methodes
    QString getQemuOutputForParameter(const QString& flag) ;

public:

    //static flags
    static const char* FLAG_MACHINE;
    static const char* FLAG_CPU;
    static const char* FLAG_MEMORY_MB;
    static const char* FLAG_KERNAL;
    static const char* FLAG_HARDDRIVE;
    static const char* FLAG_CDROM;
    static const char* FLAG_BOOT;
    static const char* FLAG_NET;
    static const char* FLAG_APPEND;
    static const char* FLAG_NOGRAPHIC;
};

#endif // QEMUSENSORMODEL_H
