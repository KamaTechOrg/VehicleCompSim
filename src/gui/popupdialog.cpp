#include "popupdialog.h"
#include "CMakeUtils/getBuildAndRunCommands.h"
#include "ui_popupdialog.h"

PopupDialog::PopupDialog(SensorModel* oldSensorModel, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PopupDialog)
    , oldSensorModel(oldSensorModel)

{
    setModal(true);
    ui->setupUi(this);
    ui->ID->setText(oldSensorModel->priority());
    ui->Name->setText(oldSensorModel->name());
    ui->BuildCommand->setText(oldSensorModel->buildCommand());
    ui->RunCommand->setText(oldSensorModel->runCommand());
    ui->cmakePath->setText(oldSensorModel->cmakePath());
    ui->isDefaultCommands->setCheckState((oldSensorModel->isUseCmakePath() ? Qt::Checked : Qt::Unchecked));
    on_isDefaultCommands_stateChanged(ui->isDefaultCommands->checkState());
    this->setWindowTitle("Sensor Data");
}

PopupDialog::~PopupDialog()
{
    delete ui;
}


void PopupDialog::on_Save_Button_clicked()
{
    oldSensorModel->setPriority(ui->ID->text());
    oldSensorModel->setName(ui->Name->text());

    if (ui->isDefaultCommands->isChecked())
    {
        auto commands = CMakeUtils::getBuildAndRunCommands(ui->cmakePath->text());
        // folow-up: throw an error
        oldSensorModel->setBuildCommand(commands.first);
        oldSensorModel->setRunCommand(commands.second);
    }
    else
    {
        oldSensorModel->setBuildCommand(ui->BuildCommand->text());
        oldSensorModel->setRunCommand(ui->RunCommand->text());
    }

    oldSensorModel->setCmakePath(ui->cmakePath->text());
    oldSensorModel->setisUseCmakePath(ui->isDefaultCommands->isChecked());

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

