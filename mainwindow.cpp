#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "popupdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    PopupDialog popup(this, A, B, C, D);
    connect(&popup, &PopupDialog::dataUpdated, this, &MainWindow::updateData);
    popup.setModal(true);
    popup.exec();
}

void MainWindow::updateData(const QString &id, const QString &name, const QString &family, const QString &phone)
{
    A = id;
    B = name;
    C = family;
    D = phone;
    // qDebug() << "Updated data:" << A << B << C << D;
}


void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << "Updated data:" << A << B << C << D;
}

