#include "SimulationControlPanel.h"
#include "SimulationReplayer.h"

SimulationControlPanel::SimulationControlPanel(SimulationReplayer* replayer, QWidget* parent)
        : QWidget(parent), m_replayer(replayer), m_isManualSliderChange(false) {
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
    connect(m_slider, &QSlider::sliderPressed, this, &SimulationControlPanel::onSliderPressed);
    connect(m_slider, &QSlider::sliderReleased, this, &SimulationControlPanel::onSliderReleased);

    int delay = qMax(0, m_replayer->m_startTime.msecsTo(m_replayer->m_totalTime));
    delayTime = QTime(0, 0).addMSecs(delay);
    m_totalTimeLabel->setText(delayTime.toString("hh:mm:ss"));
    m_totalTimeLabel->update();
    m_elapsedTimeLabel->update();
    m_slider->setRange(0, delay/1000);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &SimulationControlPanel::updateSlider);
    m_updateTimer->start(1000);
    pause(false);
}

void SimulationControlPanel::play() {
    play_mode = true;
    m_replayer->playSimulation();
    m_updateTimer->start();
}

void SimulationControlPanel::pause(bool from_slider_pressed) {
    if(!from_slider_pressed){
        play_mode = false;
    }
    m_replayer->pauseSimulation();
    m_updateTimer->stop();
}

void SimulationControlPanel::seek(int value) {
    QTime targetTime(0, 0, 0);
    targetTime = targetTime.addSecs(value);
    currentTime = targetTime;
    m_elapsedTimeLabel->setText(currentTime.toString("hh:mm:ss"));
    m_elapsedTimeLabel->update();

    m_replayer->jumpToTime(targetTime, m_isManualSliderChange);
    if(!play_mode){
        pause(false);
    }
}

void SimulationControlPanel::updateSlider() {
    if (currentTime >= delayTime) {
        pause(false);
        return;
    }
    currentTime = currentTime.addSecs(1);
    m_elapsedTimeLabel->setText(currentTime.toString("hh:mm:ss"));
    m_elapsedTimeLabel->update();
    int sliderValue = QTime(0, 0, 0).secsTo(currentTime);
    m_slider->setValue(sliderValue);
}

void SimulationControlPanel::onSliderPressed() {
    pause(true);
    m_isManualSliderChange = true;
}

void SimulationControlPanel::onSliderReleased() {
    if(play_mode){
        play();
    }
    m_isManualSliderChange = false;
}