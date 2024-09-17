#include "./SensorItem_Editor.h"
#include <vector>

void SensorItem::Editor::initPriority()
{
    priority->setText(model.priority());
    QObject::connect(priority, &QLineEdit::textChanged, [this](){
        model.setPriority(priority->text());
    });
    layout->addWidget(labels[priority] = new QLabel("priority", this));
    layout->addWidget(priority);
}

void SensorItem::Editor::initName()
{
    name->setText(model.name());
    QObject::connect(name,  &QLineEdit::textChanged  , [this](){
        model.setName(name->text());
    });
    layout->addWidget(labels[name] = new QLabel("name", this));
    layout->addWidget(name);
}

void SensorItem::Editor::initBuildCommand()
{
    buildCommand->setText(model.buildCommand());
    QObject::connect(buildCommand,  &QLineEdit::textChanged  , [this](){
        model.setBuildCommand(buildCommand->text());
    });
    layout->addWidget(labels[buildCommand] = new QLabel("build Command", this));
    layout->addWidget(buildCommand);
}

void SensorItem::Editor::initRunCommand()
{
    runCommand->setText(model.runCommand());
    QObject::connect(runCommand,    &QLineEdit::textChanged  , [this](){
        model.setRunCommand(runCommand->text());
    });
    layout->addWidget(labels[runCommand] = new QLabel("run Command", this));
    layout->addWidget(runCommand);
}

void SensorItem::Editor::initCmakeSelectorOpen()
{
    cmakeSelectorOpen->setText(model.cmakePath());
    QObject::connect(cmakeSelectorOpen,    &QPushButton::clicked  , [this](){
        QString newCMakeFile = QFileDialog::getOpenFileName(this, "Select CMake File",
                                                            model.cmakePath(),
                                                            "CMake files (CMakeLists.txt)");
        if (!newCMakeFile.isEmpty()) model.setCmakePath(newCMakeFile);
        cmakeSelectorOpen->setText(model.cmakePath());
    });
    layout->addWidget(labels[cmakeSelectorOpen] = new QLabel("Select cmake file", this));
    layout->addWidget(cmakeSelectorOpen);
}

void SensorItem::Editor::initIsUseCmakePath()
{
    isUseCmakePath->setCheckState((model.isUseCmakePath() ? Qt::Checked : Qt::Unchecked));
    QObject::connect(isUseCmakePath,&QCheckBox::checkStateChanged , [this](){
        model.setisUseCmakePath((isUseCmakePath->checkState() == Qt::Checked));
        switchProjectInputMethod();
    });
    layout->addWidget(labels[isUseCmakePath] = new QLabel("use cmake file as input", this));
    layout->addWidget(isUseCmakePath);
}

void SensorItem::Editor::initParameters()
{
    initPriority();
    initName();
    initBuildCommand();
    initRunCommand();
    initCmakeSelectorOpen();
    initIsUseCmakePath();
}

void SensorItem::Editor::initLayout()
{
    setLayout(layout);
}

void SensorItem::Editor::switchProjectInputMethod()
{
    std::vector<QWidget*> cmakePath_items = {cmakeSelectorOpen};
    std::vector<QWidget*> noCmakePath_items = {buildCommand, runCommand};

    for (auto widjet: cmakePath_items)
    {
        if (isUseCmakePath->checkState() == Qt::Checked) {
            widjet->show();
            labels[widjet]->show() ;
        }
        else
        {
            widjet->hide();
            labels[widjet]->hide() ;
        }
    }

    for (auto widjet: noCmakePath_items)
    {
        if (isUseCmakePath->checkState() != Qt::Checked) {
            widjet->show();
            labels[widjet]->show() ;
        }
        else
        {
            widjet->hide();
            labels[widjet]->hide() ;
        }
    }
}

SensorItem::Editor::Editor(SensorItem *_sensor) : model(_sensor->getModel())
{

    initParameters();
    initLayout();
    switchProjectInputMethod();

}

void SensorItem::Editor::open()
{
    this->show();
}

void SensorItem::Editor::close()
{
    qInfo() << "closing...";
}
