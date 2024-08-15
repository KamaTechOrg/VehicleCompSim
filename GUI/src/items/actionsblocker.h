#ifndef ACTIONSBLOCKER_H
#define ACTIONSBLOCKER_H

#include <QWidget>

class ActionsBlocker : public QWidget
{
public:
    explicit ActionsBlocker(QWidget *parent = nullptr);
    void transparency(float precentage);
signals:
};

#endif // ACTIONSBLOCKER_H
