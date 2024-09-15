#ifndef VEHICLECOMPSIM_BUFFER_TEST_H
#define VEHICLECOMPSIM_BUFFER_TEST_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include <QDir>
#include <QTextStream>
#include <QDebug>

class buffer_test : public QObject {
Q_OBJECT

public:
    explicit buffer_test(QObject *parent = nullptr);
    ~buffer_test();

public slots:
    void generate_buffer();

private:
    QList<QString> msg = {"yossi goldberg", "naomi goldberg", "daniel goldberg", "nechemia goldberg", "avishay goldberg"};
    QFile m_logFile;
    QTimer *m_timer;
    int msg_counter;
};

#endif //VEHICLECOMPSIM_BUFFER_TEST_H