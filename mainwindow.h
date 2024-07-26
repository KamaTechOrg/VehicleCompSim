#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "popupdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void updateData(const QString &id, const QString &name, const QString &family, const QString &phone);


    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QString A = "yossi";
    QString B = "goldberg";
    QString C = "0504176037";
    QString D = "050417603dddd";
};
#endif // MAINWINDOW_H
