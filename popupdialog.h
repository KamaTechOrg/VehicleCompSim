#ifndef POPUPDIALOG_H
#define POPUPDIALOG_H

#include <QDialog>
#include <QString>


namespace Ui {
class PopupDialog;
}

class PopupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PopupDialog(QWidget *parent = nullptr, const QString &id= nullptr, const QString & name= nullptr, const QString & family= nullptr, const QString  & phone= nullptr);
    ~PopupDialog();
signals:
    void dataUpdated(const QString &id, const QString &name, const QString &family, const QString &phone);


private slots:
    void on_Save_Button_clicked();

    void on_Cancel_Button_clicked();

    void on_Save_Button_clicked(bool checked);

private:
    Ui::PopupDialog *ui;
};

#endif // POPUPDIALOG_H
