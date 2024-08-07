#include "actionsblocker.h"

ActionsBlocker::ActionsBlocker(QWidget *parent)
    : QWidget{parent}
{
    setStyleSheet("background-color: rgba(128, 128, 128, 0.1);");
    setGeometry(0, 0, 1000, 1000);
    raise();
}

void ActionsBlocker::transparency(float precentage)
{
    setStyleSheet(QString() + "background-color: rgba(128, 128, 128, " + QString::number(precentage)  + ");");

}
