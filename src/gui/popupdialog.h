#pragma once

#include <QDialog>
#include <QString>
#include "items/sensoritem.h"

namespace Ui {
class PopupDialog;
}

class PopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PopupDialog(SensorItem* oldSensorItem, QWidget *parent = nullptr);
    ~PopupDialog();

private slots:
    void on_Save_Button_clicked();

    void on_Cancel_Button_clicked();

private:
    Ui::PopupDialog *ui;
    SensorItem* oldSensorItem;
};
