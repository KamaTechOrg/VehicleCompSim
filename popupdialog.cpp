#include "PopupDialog.h"
#include "ui_popupdialog.h"

PopupDialog::PopupDialog(QWidget *parent)
    : QDialog(parent),
ui(new Ui::PopupDialog)
{
    ui->setupUi(this);
}
PopupDialog::~PopupDialog()
{
    delete ui;
}
