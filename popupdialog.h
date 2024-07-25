#ifndef POPUPDIALOG_H
#define POPUPDIALOG_H

#include <QDialog>

namespace Ui {
class PopupDialog;
}

class PopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PopupDialog(QWidget *parent = nullptr);
    ~PopupDialog();

private:
    Ui::PopupDialog *ui;
};

#endif // POPUPDIALOG_H
