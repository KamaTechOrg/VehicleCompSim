//
// Created by OWNER on 11/08/2024.
//

#ifndef VEHICLECOMPSIM_SIMULATIONCONTROLPANEL_H
#define VEHICLECOMPSIM_SIMULATIONCONTROLPANEL_H


#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>

class SimulationReplayer;

class SimulationControlPanel : public QWidget {
    Q_OBJECT

public:
    SimulationControlPanel(SimulationReplayer* replayer, QWidget* parent = nullptr);

private slots:
    void play();
    void pause(bool from_slider_pressed);
    void seek(int value);
    void updateSlider();
    void onSliderPressed();
    void onSliderReleased();

private:
    SimulationReplayer* m_replayer;
    QPushButton* m_playButton;
    QPushButton* m_pauseButton;
    QSlider* m_slider;
    QLabel* m_elapsedTimeLabel;
    QLabel* m_totalTimeLabel;
    QTimer* m_updateTimer;
    QTime currentTime = (QDate(1970, 1, 1), QTime(0, 0, 0));
    QTime delayTime;
    bool m_isManualSliderChange;
    bool play_mode = false;



};



#endif //VEHICLECOMPSIM_SIMULATIONCONTROLPANEL_H
