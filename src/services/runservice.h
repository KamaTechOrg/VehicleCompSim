#ifndef RUNSERVICE_H
#define RUNSERVICE_H

#include "../gui/customscene.h"
#include <QProcess>
#include <future>

#ifdef _WIN32
#include <minwindef.h>
#endif

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
        int successfulStartedProcesses = 0;
        int successfulFinishedProcesses = 0;
        int timer = 0;
        std::future<void> timer_async_return;
        void start(bool simultaneously = false);
        void stop();
        void startTimer();
        void reset();
    };

    CustomScene* scene;
    QVector<SensorItem*> sensors;
    RunControllData runControl;
    int timer = 0;
    bool isRunning = false;

    std::shared_ptr<QProcess> processInit(const QString &program, const QStringList& arguments);
    std::shared_ptr<QProcess> processInit(const QString &command);
    void extarctSensorsFromScene();
    void compile();
    void run();
    void dump(unsigned long long pid);
    void runAndBuildServer();



public:
    RunService();
    ~RunService();

    void setScene(CustomScene* _scene);
    void start();
    void start(const std::function<void ()> onFinish);
    void stop();
    void setTimer(int t);

};

#endif // RUNSERVICE_H
