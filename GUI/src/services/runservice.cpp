#include "runservice.h"
#include <thread>
#include <QSettings>
#include "processControls.h"
#include "createDump.h"
#include "globalstate.h"

int RunService::RunControllData::RunControllCounter = 0;

RunService::RunService()
    : timer(0)
{}

RunService::~RunService()
{
    isRunning = false;
}

void RunService::start()
{
    isRunning = true;
    extarctSensorsFromScene();

    compile();
}

void RunService::start(const std::function<void(void)> onFinish)
{
    isRunning = true;
    auto listner = std::thread([onFinish, this](){
        while (this->isRunning) { }
        onFinish();
    });
    listner.detach();
    start();
}

void RunService::stop()
{    
    runControl.stop();
    isRunning = false;
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
            qDebug().noquote() << "Error occurred: " << error << '\n';
            qDebug().noquote() << "Error string: " << process_ptr->errorString();
        }
    });

    QObject::connect(process_ptr.get(), &QProcess::readyReadStandardOutput, [weak_process_ptr](){
        if (auto process_ptr = weak_process_ptr.lock()) {
            qDebug().noquote() << process_ptr->readAllStandardOutput();
        }
    });

    QObject::connect(process_ptr.get(), &QProcess::readyReadStandardError, [weak_process_ptr](){
        if (auto process_ptr = weak_process_ptr.lock()) {
            qDebug().noquote() << process_ptr->readAllStandardError();
        }
    });


    return process_ptr;
}

std::shared_ptr<QProcess> RunService::processInit(const QString &command)
{
    // Start the process with a shell to interpret the combined command
// #ifdef Q_OS_WIN
//     return processInit("cmd.exe", {"/c", command});
// #else
//     return processInit("sh", {"-c", command});
// #endif

    QStringList list = command.split(' ');
    QString program = list.takeFirst();

    return processInit(program, list);
}

void RunService::extarctSensorsFromScene()
{
    QSettings settings("VehicleCompSim", "GUI");
    QString clientId = settings.value("clientId").toString();
    sensors.clear();
    for (auto & model: GlobalState::getInstance().currentProject()->models())
    {
        SensorModel* sensor = dynamic_cast<SensorModel*>(model);
        if (sensor && !sensor->isExcludeFromProject() && sensor->ownerID() == clientId)
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
        if (sensor->buildCommand().isEmpty()) continue;

        auto process = processInit(sensor->buildCommand());
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
        auto process = processInit(sensor->runCommand());
        auto process_ptr = process.get();
        QObject::connect(process_ptr, &QProcess::errorOccurred, [process_ptr, this](QProcess::ProcessError error){
            this->runControl.isErrorAccure = true;
            //this->dump(process_ptr->processId());

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
    runControl.start(true);

}

void RunService::dump(unsigned long long pid)
{
    std::string dumpOutputPath = (QDir::currentPath() + "/dumps/" + QString::number(pid) + ".dump").toStdString();
    //CreateProcessDump(pid, dumpOutputPath.toStdString());
//    std::system((std::string("C:/QT PROJECTS/TEST/GUI/VehicleCompSim/externalLibraries/Procdump/procdump.exe -e 1 -f \"\" -ma ") + QString::number(pid).toStdString() + " " + dumpOutputPath).c_str());
    //qInfo() << QDir::currentPath();

}



void RunService::RunControllData::start(bool simultaneously)
{
    if (isStarted) return;
    isRunning = true;
    auto runControlId = this->controllerId;
    for (auto& process : sensorsProcesses)
    {
        process->start();
        if (simultaneously)
        {
            std::thread([&process, runControlId, this](){
                process->waitForStarted();
                suspendProcess(process->processId());
                if (this->controllerId == runControlId) ++this->successfulStartedProcesses;
                while (this->controllerId == runControlId && this->successfulStartedProcesses != this->sensorsProcesses.size());

                if (this->controllerId == runControlId) resumeProcess(process->processId());
            }).detach();
        }
        std::string dumpOutputPath = (QString("C:/Procdump") + "/dumps/" + QString::number(process->processId()) + ".dump").toStdString();
        //CreateProcessDump(pid, dumpOutputPath.toStdString());
        //std::system((std::string("C:/Procdump/procdump.exe -e 1 -f \"\" -ma ") + QString::number(process->processId()).toStdString() + " " + dumpOutputPath).c_str());
        //qInfo() << QDir::currentPath();
    }
    isStarted = true;
    if (isErrorAccure) return;

    startTimer();
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

void RunService::RunControllData::startTimer()
{
    auto currentControllerId = this->controllerId;
    if (timer > 0)
    {
        timer_async_return = std::async(std::launch::async, [this, currentControllerId]() {
            std::this_thread::sleep_for(std::chrono::seconds(this->timer));
            if (this->controllerId == currentControllerId) this->stop();
        });
    }
}

void RunService::RunControllData::reset()
{
    *this = RunService::RunControllData();
}


