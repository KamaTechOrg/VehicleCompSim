#include "SimulationControlPanel.h"
#include "SimulationReplayer.h"

SimulationControlPanel::SimulationControlPanel(SimulationReplayer* replayer, QWidget* parent)
: QWidget(parent), m_replayer(replayer){
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
    int delayMs = qMax(0, m_replayer->m_startTime.msecsTo(m_replayer->m_totalTime));
    m_totalTime = QTime(0, 0).addMSecs(delayMs);
    m_slider->setRange(0, delayMs);
    m_totalTimeLabel->setText(QTime(0, 0).addMSecs(delayMs).toString("hh:mm:ss.zzz"));
    m_elapsedTimeLabel->setText("00:00:00.000");
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &SimulationControlPanel::updateSlider);
    m_updateTimer->start(100);  // Update every 100 ms
    pause();
}

void SimulationControlPanel::play() {
    if(m_isPlaying){
        return;
    }
    m_isPlaying = true;
    m_replayer->playSimulation();
    m_updateTimer->start();
}

void SimulationControlPanel::pause() {
    if(!m_isPlaying){
        return;
    }
    m_isPlaying = false;
    m_replayer->pauseSimulation();
    m_updateTimer->stop();
}

void SimulationControlPanel::seek(int value) {
    QTime targetTime = QTime(0, 0).addMSecs(value);
    m_currentTime = targetTime;
    m_elapsedTimeLabel->setText(targetTime.toString("hh:mm:ss.zzz"));
    m_slider->blockSignals(true);
    m_slider->setValue(QTime(0, 0).msecsTo(m_currentTime));
    m_slider->blockSignals(false);
    m_replayer->jumpToTime(targetTime);
    if(!m_isPlaying){
        m_isPlaying = true;
        pause();
    }
}

void SimulationControlPanel::updateSlider() {
    if (m_currentTime >= m_totalTime) {
        pause();
        return;
    }
    m_currentTime = m_currentTime.addMSecs(100);
    if(m_currentTime > m_totalTime){
        m_currentTime = m_totalTime;
    }
    m_elapsedTimeLabel->setText(m_currentTime.toString("hh:mm:ss.zzz"));
    m_slider->blockSignals(true);
    m_slider->setValue(QTime(0, 0).msecsTo(m_currentTime));
    m_slider->blockSignals(false);
}