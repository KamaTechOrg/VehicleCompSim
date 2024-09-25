#include "runmanager.h"

void RunManager::nextStep()
{
    if (currentStep < Runner::START){
        currentStep = Runner::RunSteps(((int)currentStep) + 1);
        if (currentStep == Runner::RUN_STEPS_COUNT) stop();
        emit step(currentStep);
    }
}

RunManager::RunManager() {}

RunManager::~RunManager()
{
    if (currentStep != Runner::ZERO || currentStep != Runner::RUN_STEPS_COUNT) stop();
}

void RunManager::addRunner(std::shared_ptr<Runner> _runner)
{
    std::weak_ptr<Runner> weak_runner = _runner;

    runners.push_back(_runner);

    QObject::connect(this, &RunManager::step, [weak_runner](Runner::RunSteps step){
        if (auto runner = weak_runner.lock())
        {
            runner->step(step);
        }
    });
    QObject::connect(this, &RunManager::abort, [weak_runner](){
        if (auto runner = weak_runner.lock())
        {
            runner->abort();
        }
    });

    QObject::connect(_runner.get(), &Runner::stepSuccess, [this](Runner::RunSteps step){
        onRunnerFinishedStep(step);
    });
    QObject::connect(_runner.get(), &Runner::errorAccure, [this](){
        onRunnerAccureError();
    });
}

void RunManager::start()
{
    if (currentStep == Runner::ZERO)
    {
        nextStep();
    }
}

void RunManager::stop()
{
    emit abort();
    emit stopFinished();
}

void RunManager::onRunnerFinishedStep(Runner::RunSteps step)
{
    if (step != currentStep )
    {
        return stop();
    }

    ++countRunnersFinishedStep;

    if (countRunnersFinishedStep == runners.size())
    {
        nextStep();
    }
}

void RunManager::onRunnerAccureError()
{
    stop();
}
