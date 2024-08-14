//
// Created by OWNER on 11/08/2024.
//

#include "SimulationControlPanel.h"
#include "SimulationReplayer.h"

SimulationControlPanel::SimulationControlPanel(SimulationReplayer* replayer, QWidget* parent)
        : QWidget(parent), m_replayer(replayer) {
    m_playButton = new QPushButton("Play");
    m_pauseButton = new QPushButton("Pause");
    m_slider = new QSlider(Qt::Horizontal);
    m_elapsedTimeLabel = new QLabel("00:00:00");
    m_totalTimeLabel = new QLabel("00:00:00");

    QHBoxLayout* controlLayout = new QHBoxLayout;
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(m_pauseButton);
    controlLayout->addWidget(m_slider);
    controlLayout->addWidget(m_elapsedTimeLabel);
    controlLayout->addWidget(m_totalTimeLabel);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(controlLayout);
    setLayout(mainLayout);
    connect(m_playButton, &QPushButton::clicked, this, &SimulationControlPanel::play);
    connect(m_pauseButton, &QPushButton::clicked, this, &SimulationControlPanel::pause);
    connect(m_slider, &QSlider::valueChanged, this, &SimulationControlPanel::seek);
    int delay = qMax(0, m_replayer->m_startTime.msecsTo(m_replayer->m_totalTime));
    delayTime = QTime(0, 0).addMSecs(delay);
    m_totalTimeLabel->setText(delayTime.toString("hh:mm:ss"));
    m_totalTimeLabel->update();
    m_elapsedTimeLabel->update();
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &SimulationControlPanel::updateSlider);
    m_updateTimer->start(1000);
}

void SimulationControlPanel::play() {
    m_replayer->playSimulation();
    m_updateTimer->start();
}

void SimulationControlPanel::pause() {
    m_replayer->pauseSimulation();
    m_updateTimer->stop();

}

void SimulationControlPanel::seek(int value) {
    QTime start = (QDate(1970, 1, 1), QTime(0, 0, 0));
    QTime targetTime = start.addSecs(value);
    currentTime = targetTime;
    m_elapsedTimeLabel->setText(currentTime.toString("hh:mm:ss"));
    m_elapsedTimeLabel->update();
    m_replayer->jumpToTime(targetTime);
}

void SimulationControlPanel::updateSlider() {
    if (currentTime >= delayTime) {
        pause();
        return;
    }
    QTime currentTime_2 = currentTime.addSecs(1);
    currentTime = currentTime_2;
    m_elapsedTimeLabel->setText(currentTime.toString("hh:mm:ss"));
    m_elapsedTimeLabel->update();
    int sliderValue = currentTime.hour() * 3600 + currentTime.minute() * 60 + currentTime.second();
    m_slider->setValue(sliderValue);
}