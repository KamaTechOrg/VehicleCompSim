#include "popupdialog.h"
#include "ui_popupdialog.h"

PopupDialog::PopupDialog(QWidget *parent, const QString &old_id, const QString &old_name, const QString &old_family, const QString & old_number)
    : QDialog(parent)
    , ui(new Ui::PopupDialog)

{
    ui->setupUi(this);
    ui->ID->setText(old_id);
    ui->Name->setText(old_name);
    ui->Family->setText(old_family);
    ui->Phone->setText(old_number);


}

PopupDialog::~PopupDialog()
{
    delete ui;
}


void PopupDialog::on_Save_Button_clicked()
{
    emit dataUpdated(ui->ID->text(), ui->Name->text(), ui->Family->text(), ui->Phone->text()); // Emit the signal with updated data
    accept(); // Close the dialog
}


void PopupDialog::on_Cancel_Button_clicked()
{
    accept(); // Close the dialog
}


void PopupDialog::on_Save_Button_clicked(bool checked)
{

}

