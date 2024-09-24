
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
    int random_src();
    int random_dest(int src);
    int random_int();
    int random_double();
    std::string random_msg();
    std::string timeStamp();
    void sensor1();
    void sensor2();
    void sensor3();
    void sensor4();

    ~buffer_test();

public slots:
    void my_new_buffer();

private:
    std::vector<std::string> msg = {"Alice", "Bobbb", "Charl", "David", "Eveee",
                          "Frank", "Grace", "Hanna", "Isaac", "Julia", "yossi", "naomi",
                          "danie", "neche", "avish"};
    QFile m_logFile;
    QTimer *m_timer;
    int msg_counter;
    QMap<int, int> my_map = {{1,3}, {2,3}, {3,2}, {4,2}, {5,4}, {6,1}, {7,1}, {8,5}, {9,5}, {10,4}, {11,5}};

};
#endif //VEHICLECOMPSIM_BUFFER_TEST_H
