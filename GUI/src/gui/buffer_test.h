
#ifndef VEHICLECOMPSIM_BUFFER_TEST_H
#define VEHICLECOMPSIM_BUFFER_TEST_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QRandomGenerator>
#include <Vector>


class buffer_test : public QObject {
Q_OBJECT

public:
    explicit buffer_test(QObject *parent = nullptr);
    void testExtractBufferData();
    ~buffer_test();
    void start_timer();
    void stop_timer();
    QTimer *m_buffer_test_timer;


};
#endif //VEHICLECOMPSIM_BUFFER_TEST_H
