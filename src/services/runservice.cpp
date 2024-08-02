#include "runservice.h"
#include <thread>

int RunService::RunControllData::RunControllCounter = 0;

RunService::RunService()
    : scene(nullptr), timer(0)
{}

void RunService::setScene(CustomScene *_scene)
{
    scene = _scene;
}

void RunService::start()
{

    extarctSensorsFromScene();

    compile();
}

void RunService::stop()
{    
    runControl.stop();
}

void RunService::setTimer(int t)
{
    timer = t;
}


std::shared_ptr<QProcess> RunService::processInit(const QString &program, const QStringList& arguments)
{
    std::shared_ptr<QProcess> process_ptr(new QProcess);
    std::weak_ptr<QProcess> weak_process_ptr(process_ptr); // waek_ptr is used to safly calling the qprocess withoout incrementing its ptr count

    process_ptr->setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    process_ptr->setProgram(program);
    process_ptr->setArguments(arguments);

    QObject::connect(process_ptr.get(), &QProcess::errorOccurred, [weak_process_ptr](QProcess::ProcessError error){
        if (auto process_ptr = weak_process_ptr.lock()) {
            qDebug() << "Error occurred: " << error << '\n';
            qDebug() << "Error string: " << process_ptr->errorString();
        }
    });

    QObject::connect(process_ptr.get(), &QProcess::readyReadStandardOutput, [weak_process_ptr](){
        if (auto process_ptr = weak_process_ptr.lock()) {
            qDebug() << process_ptr->readAllStandardOutput();
        }
    });

    QObject::connect(process_ptr.get(), &QProcess::readyReadStandardError, [weak_process_ptr](){
        if (auto process_ptr = weak_process_ptr.lock()) {
            qDebug() << process_ptr->readAllStandardError();
        }
    });


    return process_ptr;
}

std::shared_ptr<QProcess> RunService::processInit(const QString &command)
{
    QStringList list = command.split(' ');
    QString program = list.takeFirst();

    return processInit(program, list);
}

void RunService::extarctSensorsFromScene()
{
    sensors.clear();
    for (auto & item: scene->items())
    {
        if (SensorItem* sensor = dynamic_cast<SensorItem*>(item))
        {
            sensors.push_back(sensor);
        }
    }
}

void RunService::compile()
{
    if (runControl.isRunning || sensors.empty()) return;

    runControl = RunControllData();

    for (auto sensor: sensors)
    {
        if (sensor->getBuildCommand().isEmpty()) continue;

        auto process = processInit(sensor->getBuildCommand());
        auto process_ptr = process.get();

        QObject::connect(process_ptr, &QProcess::errorOccurred, [process_ptr, this](QProcess::ProcessError error){
            this->runControl.isErrorAccure = true;
        });

        QObject::connect(process_ptr, &QProcess::finished, [process_ptr, this](int exitCode, QProcess::ExitStatus exitStatus){
            if (exitStatus == QProcess::NormalExit && exitCode == 0) {

                if (++this->runControl.successfulFinishedProcesses == this->runControl.sensorsProcesses.size())
                {

                    this->runControl.stop();
                    this->run();
                }

            }
        });

        runControl.sensorsProcesses.push_back(process);
    }
    runControl.start();

}

void RunService::run()
{
    if (runControl.isRunning || sensors.empty()) return;

    runControl.reset();

    for (auto sensor: sensors)
    {
        auto process = processInit(sensor->getRunCommand());
        auto process_ptr = process.get();

        QObject::connect(process_ptr, &QProcess::errorOccurred, [process_ptr, this](QProcess::ProcessError error){
            this->runControl.isErrorAccure = true;
        });

        QObject::connect(process_ptr, &QProcess::finished, [process_ptr, this](int exitCode, QProcess::ExitStatus exitStatus){
     //       if (exitStatus == QProcess::NormalExit && exitCode == 0) {
                if (++this->runControl.successfulFinishedProcesses == this->runControl.sensorsProcesses.size())
                {
                    this->runControl.isFinished = true;
                    this->runControl.isRunning = false;

                }
      //      }
        });

        runControl.sensorsProcesses.push_back(process);
    }
    runControl.timer = timer;
    runControl.start();

}

void RunService::RunControllData::start()
{

    isRunning = true;
    for (auto& process : sensorsProcesses) process->start();
    isStarted = true;
    if (isErrorAccure) return;

    auto currentControllerId = this->controllerId;
    if (timer > 0)
    {
        timer_async_return = std::async(std::launch::async, [this, currentControllerId]() {
            std::this_thread::sleep_for(std::chrono::seconds(this->timer));
            if (this->controllerId == currentControllerId) this->stop();
        });
    }
}

void RunService::RunControllData::stop()
{

    if (!isStarted) return;
    for (auto& process: sensorsProcesses)
    {
        //process->terminate();
        //process->kill();
        process->close();
    }
    isRunning = false;
}

void RunService::RunControllData::reset()
{
    *this = RunService::RunControllData();
}
