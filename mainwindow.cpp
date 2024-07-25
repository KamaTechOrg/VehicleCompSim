#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "popupdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    popupDialog(new PopupDialog(this))

{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    popupDialog; popupDialog;
    popupDialog->setModal(true);
    popupDialog->exec(); // Show the popup dialog modally
}
