#ifndef RUNSERVICE_H
#define RUNSERVICE_H

#include "../gui/customscene.h"
#include "../include/VehicleCompSim/utils/runCommand/runCommand.h"
#include <future>

class RunService
{

    struct RunControllData
    {
        static int RunControllCounter;
        int controllerId = ++RunControllCounter;
        QVector<std::shared_ptr<QProcess>> sensorsProcesses;
        bool isRunning = false;
        bool isStarted = false;
        bool isFinished = false;
        bool isErrorAccure = false;
        int successfulFinishedProcesses = 0;
        int timer = 0;
        std::future<void> timer_async_return;
        void start();
        void stop();
        void reset();
    };

    CustomScene* scene;
    QVector<SensorItem*> sensors;
    RunControllData runControl;
    int timer;

    std::shared_ptr<QProcess> processInit(const QString &program, const QStringList& arguments);
    std::shared_ptr<QProcess> processInit(const QString &command);
    void extarctSensorsFromScene();
    void compile();
    void run();
public:
    RunService();

    bool running = false;



    void setScene(CustomScene* _scene);
    void start();
    void stop();
    void setTimer(int t);



};

#endif // RUNSERVICE_H
