#pragma once

#include <QString>
#include <QtWidgets/QDialog>
#include <QLineEdit>
#include <QToolBar>
#include "items/sensoritem.h"

class PopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PopupDialog(QToolBar* rightToolBar, QWidget *parent = nullptr);
    void showContentInRightToolbar();
    void reset();
    SensorItem* oldSensorItem = nullptr;
    bool cancel = false;

private slots:
    void on_Save_Button_clicked();
    void on_Cancel_Button_clicked();
private:
    void showInputDialogForCMakePath();
    void showInputDialogForName();
    void showInputDialogForID();
    void showInputDialogForBuildCommand();
    void showInputDialogForRunCommand();
    void handleCMakePathTextChanged(QString newText);
    void handleNameTextChanged(QString newText);
    void handleIdTextChanged(QString newText);
    void handleBuildCommandTextChanged(QString newText);
    void handleRunCommandTextChanged(QString newText);
    void on_Change_Button_clicked();

    QToolBar* rightToolBar;
    QPushButton *Button1;
    QPushButton *Button2;
    QPushButton *Button3;
    QPushButton *Button4;
    QPushButton *Button5;
    QPushButton *Button6;
    QPushButton *Button7;
    QPushButton *Button8;
    QLineEdit *inputBox1;
    QLineEdit *inputBox2;
    QLineEdit *inputBox3;
    QLineEdit *inputBox4;
    QLineEdit *inputBox5;
    QString cmake;
    QString id;
    QString name;
    QString BuildCommand;
    QString RunCommand;
    QWidget* buttonContainer;
    bool first = true;
    bool change = true;
};
