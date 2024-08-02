#include "popupdialog.h"
#include "VehicleCompSim/utils/CMakeUtils/getBuildAndRunCommands.h"
#include "ui_popupdialog.h"

PopupDialog::PopupDialog(SensorItem* oldSensorItem, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PopupDialog)
    , oldSensorItem(oldSensorItem)

{
    setModal(true);
    ui->setupUi(this);
    ui->ID->setText(oldSensorItem->getID());
    ui->Name->setText(oldSensorItem->getName());
    ui->BuildCommand->setText(oldSensorItem->getBuildCommand());
    ui->RunCommand->setText(oldSensorItem->getRunCommand());
    ui->cmakePath->setText(oldSensorItem->getCmakePath());
    ui->isDefaultCommands->setCheckState((oldSensorItem->isUseCmakePath() ? Qt::Checked : Qt::Unchecked));
}

PopupDialog::~PopupDialog()
{
    delete ui;
}


void PopupDialog::on_Save_Button_clicked()
{
    oldSensorItem->setID(ui->ID->text());
    oldSensorItem->setName(ui->Name->text());

    if (ui->isDefaultCommands->isChecked())
    {
        auto commands = CMakeUtils::getBuildAndRunCommands(ui->cmakePath->text());
        // folow-up: throw an error
        oldSensorItem->setBuildCommand(commands.first);
        oldSensorItem->setRunCommand(commands.second);
    }
    else
    {
        oldSensorItem->setBuildCommand(ui->BuildCommand->text());
        oldSensorItem->setRunCommand(ui->RunCommand->text());
    }

    oldSensorItem->setCmakePath(ui->cmakePath->text());
    oldSensorItem->setUseCmakePath(ui->isDefaultCommands->isChecked());

    accept();
}


void PopupDialog::on_Cancel_Button_clicked()
{
    accept(); // Close the dialog
}



void PopupDialog::on_isDefaultCommands_stateChanged(int arg1)
{
    if (arg1)
    {
        ui->BuildCommand->hide();
        ui->labelBuildCommand->hide();
        ui->RunCommand->hide();
        ui->labelRunCommad->hide();

        ui->cmakePath->show();
        ui->labelCmakePath->show();
    }
    else
    {

        ui->BuildCommand->show();
        ui->labelBuildCommand->show();
        ui->RunCommand->show();
        ui->labelRunCommad->show();

        ui->cmakePath->hide();
        ui->labelCmakePath->hide();
    }
}

