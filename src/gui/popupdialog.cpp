#include "popupdialog.h"
#include "ui_popupdialog.h"

PopupDialog::PopupDialog(SensorItem* oldSensorItem, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PopupDialog)
    , oldSensorItem(oldSensorItem)

{
    setModal(true);
    ui->setupUi(this);
    ui->ID->setText(oldSensorItem->getPriority());
    ui->Name->setText(oldSensorItem->getName());
    ui->BuildCommand->setText(oldSensorItem->getBuildCommand());
    ui->RunCommand->setText(oldSensorItem->getRunCommand());

}

PopupDialog::~PopupDialog()
{
    delete ui;
}


void PopupDialog::on_Save_Button_clicked()
{
    oldSensorItem->setPriority(ui->ID->text());
    oldSensorItem->setName(ui->Name->text());
    oldSensorItem->setBuildCommand(ui->BuildCommand->text());
    oldSensorItem->setRunCommand(ui->RunCommand->text());
    accept(); 
}


void PopupDialog::on_Cancel_Button_clicked()
{
    accept(); // Close the dialog
}


