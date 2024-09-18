
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

class buffer_test : public QObject {
Q_OBJECT

public:
    explicit buffer_test(QObject *parent = nullptr);
    ~buffer_test();

public slots:
    void generate_buffer();

private:
    QList<QString> msg = {"Alice", "Bob", "Charlie", "David", "Eve",
                          "Frank", "Grace", "Hannah", "Isaac", "Julia", "yossi", "naomi",
                          "daniel", "nechemia", "avishay"};
    QFile m_logFile;
    QTimer *m_timer;
    int msg_counter;
    QMap<int, int> my_map = {{1,3}, {2,3}, {3,2}, {4,2}, {5,4}, {6,1}, {7,1}, {8,5}, {9,5}, {10,4}, {11,5}};

};
#endif //VEHICLECOMPSIM_BUFFER_TEST_H
