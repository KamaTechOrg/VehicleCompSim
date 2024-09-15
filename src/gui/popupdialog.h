#pragma once

//#include <QDialog>
#include <QString>
#include <QtWidgets/QDialog>
#include "sensormodel.h"

namespace Ui {
class PopupDialog;
}

class PopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PopupDialog(SensorModel* oldSensorModel, QWidget *parent = nullptr);
    ~PopupDialog();

private slots:
    void on_Save_Button_clicked();

    void on_Cancel_Button_clicked();

    void on_isDefaultCommands_stateChanged(int arg1);

private:
    Ui::PopupDialog *ui;
    SensorModel* oldSensorModel;
};


// #pragma once

// #include <QString>
// #include <QtWidgets/QDialog>
// <<<<<<< GUI-Refactor
// #include "sensormodel.h"
// =======
// #include <QLineEdit>
// #include <QToolBar>
// #include "items/sensoritem.h"
// >>>>>>> GUI

// class PopupDialog : public QDialog
// {
//     Q_OBJECT

// public:
// <<<<<<< GUI-Refactor
//     explicit PopupDialog(SensorModel* oldSensorModel, QWidget *parent = nullptr);
//     ~PopupDialog();
// =======
//     explicit PopupDialog(QToolBar* rightToolBar, QWidget *parent = nullptr);
//     void showContentInRightToolbar();
//     void reset();
//     SensorItem* oldSensorItem = nullptr;
//     bool cancel = false;
// >>>>>>> GUI

// private slots:
//     void on_Save_Button_clicked();
//     void on_Cancel_Button_clicked();
// private:
// <<<<<<< GUI-Refactor
//     Ui::PopupDialog *ui;
//     SensorModel* oldSensorModel;
// =======
//     void showInputDialogForCMakePath();
//     void showInputDialogForName();
//     void showInputDialogForID();
//     void showInputDialogForBuildCommand();
//     void showInputDialogForRunCommand();
//     void handleCMakePathTextChanged(QString newText);
//     void handleNameTextChanged(QString newText);
//     void handleIdTextChanged(QString newText);
//     void handleBuildCommandTextChanged(QString newText);
//     void handleRunCommandTextChanged(QString newText);
//     void on_Change_Button_clicked();

//     QToolBar* rightToolBar;
//     QPushButton *Button1;
//     QPushButton *Button2;
//     QPushButton *Button3;
//     QPushButton *Button4;
//     QPushButton *Button5;
//     QPushButton *Button6;
//     QPushButton *Button7;
//     QPushButton *Button8;
//     QLineEdit *inputBox1;
//     QLineEdit *inputBox2;
//     QLineEdit *inputBox3;
//     QLineEdit *inputBox4;
//     QLineEdit *inputBox5;
//     QString cmake;
//     QString id;
//     QString name;
//     QString BuildCommand;
//     QString RunCommand;
//     QWidget* buttonContainer;
//     bool first = true;
//     bool change = true;
// >>>>>>> GUI
// };
