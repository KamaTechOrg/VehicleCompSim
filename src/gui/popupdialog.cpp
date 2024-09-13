#include <QHBoxLayout>
#include "popupdialog.h"
#include "CMakeUtils/getBuildAndRunCommands.h"
#include "ui_popupdialog.h"
#include "popupdialog.h"
#include "items/sensoritem.h" // Include the full definition

// Rest of your implementation...


PopupDialog::PopupDialog(QToolBar* rightToolBar, QWidget *parent)
    : QDialog(parent)
    , rightToolBar(rightToolBar)
{

}
void PopupDialog::showContentInRightToolbar() {
    rightToolBar->clear();

    Button1 = new QPushButton("ID", rightToolBar);
    Button1->setFixedSize(150, 50);
    Button1->setStyleSheet("background-color: #DDDDDD;");


    Button2 = new QPushButton("NAME", rightToolBar);
    Button2->setFixedSize(150, 50);
    Button2->setStyleSheet("background-color: #DDDDDD;");


    Button3 = new QPushButton("CMAKE PATH", rightToolBar);
    Button3->setFixedSize(150, 50);
    Button3->setStyleSheet("background-color: #DDDDDD;");

    Button6 = new QPushButton("BUILD COMMAND", rightToolBar);
    Button6->setFixedSize(150, 50);
    Button6->setStyleSheet("background-color: #DDDDDD;");

    Button7 = new QPushButton("RUN COMMAND", rightToolBar);
    Button7->setFixedSize(150, 50);
    Button7->setStyleSheet("background-color: #DDDDDD;");

    Button8 = new QPushButton("CHANGE", rightToolBar);
    Button8->setFixedSize(150, 25);
    Button8->setStyleSheet("background-color: #FF5F44;");

    inputBox1 = new QLineEdit(this);
    inputBox1->setFixedSize(150, 25);
    inputBox1->setStyleSheet("background-color: #EEEEEE;");
    inputBox1->setContentsMargins(3, 0, 0, 0); // Adjust the left margin as needed


//    inputBox1->setAlignment(Qt::AlignRight); // You can also use Qt::AlignHCenter for center alignment


    inputBox2 = new QLineEdit(this);
    inputBox2->setFixedSize(150, 25);
    inputBox2->setStyleSheet("background-color: #EEEEEE;");
    inputBox2->setContentsMargins(3, 0, 0, 0);


    inputBox3 = new QLineEdit(this);
    inputBox3->setFixedSize(150, 25);
    inputBox3->setStyleSheet("background-color: #EEEEEE;");
    inputBox3->setContentsMargins(3, 0, 0, 0);


    inputBox4 = new QLineEdit(this);
    inputBox4->setFixedSize(150, 25);
    inputBox4->setStyleSheet("background-color: #EEEEEE;");
    inputBox4->setContentsMargins(3, 0, 0, 0);


    inputBox5 = new QLineEdit(this);
    inputBox5->setFixedSize(150, 25);
    inputBox5->setStyleSheet("background-color: #EEEEEE;");
    inputBox5->setContentsMargins(3, 0, 0, 0);


    Button4 = new QPushButton("SAVE", rightToolBar);
    Button4->setFixedSize(67, 67);
    Button4->setStyleSheet("background-color: #DDDDDD;");

    Button5 = new QPushButton("CANCEL", rightToolBar);
    Button5->setFixedSize(67, 67);
    Button5->setStyleSheet("background-color: #DDDDDD;");

    buttonContainer = new QWidget(rightToolBar);
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->addWidget(Button4);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(Button5);
    buttonContainer->setLayout(buttonLayout);

    connect(Button1, &QPushButton::clicked, this, &PopupDialog::showInputDialogForID);
    connect(Button2, &QPushButton::clicked, this, &PopupDialog::showInputDialogForName);
    connect(Button3, &QPushButton::clicked, this, &PopupDialog::showInputDialogForCMakePath);
    connect(Button6, &QPushButton::clicked, this, &PopupDialog::showInputDialogForBuildCommand);
    connect(Button7, &QPushButton::clicked, this, &PopupDialog::showInputDialogForRunCommand);
    connect(Button8, &QPushButton::clicked, this, &PopupDialog::on_Change_Button_clicked);

    connect(inputBox1, &QLineEdit::textChanged, this, &PopupDialog::handleIdTextChanged);
    connect(inputBox2, &QLineEdit::textChanged, this, &PopupDialog::handleNameTextChanged);
    connect(inputBox3, &QLineEdit::textChanged, this, &PopupDialog::handleCMakePathTextChanged);
    connect(inputBox4, &QLineEdit::textChanged, this, &PopupDialog::handleBuildCommandTextChanged);
    connect(inputBox5, &QLineEdit::textChanged, this, &PopupDialog::handleRunCommandTextChanged);

    connect(Button4, &QPushButton::clicked, this, &PopupDialog::on_Save_Button_clicked);
    connect(Button5, &QPushButton::clicked, this, &PopupDialog::on_Cancel_Button_clicked);
    if(first){
        first = false;
        rightToolBar->addWidget(Button1);
        rightToolBar->addWidget(Button2);
        rightToolBar->addWidget(Button8);
        rightToolBar->addWidget(Button3);
        rightToolBar->addWidget(buttonContainer);
    }
}

void PopupDialog::showInputDialogForID() {
    rightToolBar->clear();
    showContentInRightToolbar();
    rightToolBar->addWidget(Button1);
    rightToolBar->addWidget(inputBox1);
    inputBox1->setText(id);
    rightToolBar->addWidget(Button2);
    rightToolBar->addWidget(Button8);
    if(change){
        rightToolBar->addWidget(Button3);
    }else{
        rightToolBar->addWidget(Button6);
        rightToolBar->addWidget(Button7);
    }
    rightToolBar->addWidget(buttonContainer);
}


void PopupDialog::showInputDialogForName() {
    rightToolBar->clear();
    showContentInRightToolbar();
    rightToolBar->addWidget(Button1);
    rightToolBar->addWidget(Button2);
    rightToolBar->addWidget(inputBox2);
    inputBox2->setText(name);
    rightToolBar->addWidget(Button8);
    if(change){
        rightToolBar->addWidget(Button3);
    }else{
        rightToolBar->addWidget(Button6);
        rightToolBar->addWidget(Button7);
    }
    rightToolBar->addWidget(buttonContainer);
}

void PopupDialog::showInputDialogForCMakePath() {
    rightToolBar->clear();
    showContentInRightToolbar();
    rightToolBar->addWidget(Button1);
    rightToolBar->addWidget(Button2);
    rightToolBar->addWidget(Button8);
    rightToolBar->addWidget(Button3);
    rightToolBar->addWidget(inputBox3);
    inputBox3->setText(cmake);
    rightToolBar->addWidget(buttonContainer);
}
void PopupDialog::showInputDialogForBuildCommand(){
    rightToolBar->clear();
    showContentInRightToolbar();
    rightToolBar->addWidget(Button1);
    rightToolBar->addWidget(Button2);
    rightToolBar->addWidget(Button8);
    rightToolBar->addWidget(Button6);
    rightToolBar->addWidget(inputBox4);
    inputBox4->setText(BuildCommand);
    rightToolBar->addWidget(Button7);
    rightToolBar->addWidget(buttonContainer);
}

void PopupDialog::showInputDialogForRunCommand(){
    rightToolBar->clear();
    showContentInRightToolbar();
    rightToolBar->addWidget(Button1);
    rightToolBar->addWidget(Button2);
    rightToolBar->addWidget(Button8);
    rightToolBar->addWidget(Button6);
    rightToolBar->addWidget(Button7);
    rightToolBar->addWidget(inputBox5);
    inputBox5->setText(RunCommand);
    rightToolBar->addWidget(buttonContainer);
}

void PopupDialog::handleCMakePathTextChanged(QString newText) {
    cmake = newText;
}
void PopupDialog::handleNameTextChanged(QString newText) {
    name = newText;
}
void PopupDialog::handleIdTextChanged(QString newText) {
    id = newText;
}
void PopupDialog::handleBuildCommandTextChanged(QString newText) {
    BuildCommand = newText;
}
void PopupDialog::handleRunCommandTextChanged(QString newText) {
    RunCommand = newText;
}
void PopupDialog::on_Change_Button_clicked(){
    change = !change;
    rightToolBar->clear();
    showContentInRightToolbar();
    rightToolBar->addWidget(Button1);
    rightToolBar->addWidget(Button2);
    rightToolBar->addWidget(Button8);
    if(change){
        rightToolBar->addWidget(Button3);
    }else{
        rightToolBar->addWidget(Button6);
        rightToolBar->addWidget(Button7);
    }
    rightToolBar->addWidget(buttonContainer);
}
void PopupDialog::reset(){
    qInfo() << "reset";

    first = true;
    change = true;
    id = oldSensorItem->getPriority();
    name = oldSensorItem->getName();
    cmake = oldSensorItem->getCmakePath();
    BuildCommand = oldSensorItem->getBuildCommand();
    RunCommand = oldSensorItem->getRunCommand();
    qInfo() << "befor show";

    showContentInRightToolbar();
}

void PopupDialog::on_Save_Button_clicked()
{
    oldSensorItem->setPriority(id);
    oldSensorItem->setName(name);
    if(change){
        auto commands = CMakeUtils::getBuildAndRunCommands(cmake);
        oldSensorItem->setBuildCommand(commands.first);
        oldSensorItem->setRunCommand(commands.second);
    }else{
        oldSensorItem->setBuildCommand(BuildCommand);
        oldSensorItem->setRunCommand(RunCommand);
    }
    oldSensorItem->setCmakePath(cmake);
    rightToolBar->clear();
}


void PopupDialog::on_Cancel_Button_clicked()
{
    rightToolBar->clear();
    cancel = true;
}
