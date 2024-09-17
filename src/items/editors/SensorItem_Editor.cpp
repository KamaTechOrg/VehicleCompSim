#include "./SensorItem_Editor.h"
#include <vector>

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

    priority->setText(model.priority());
    name->setText(model.name());
    buildCommand->setText(model.buildCommand());
    runCommand->setText(model.runCommand());
    cmakeSelectorOpen->setText(model.cmakePath());
    isUseCmakePath->setCheckState((model.isUseCmakePath() ? Qt::Checked : Qt::Unchecked));


    QObject::connect(priority, &QLineEdit::textChanged, [this](){
        model.setPriority(priority->text());
    });
    QObject::connect(name,  &QLineEdit::textChanged  , [this](){
        model.setName(name->text());
    });
    QObject::connect(buildCommand,  &QLineEdit::textChanged  , [this](){
        model.setBuildCommand(buildCommand->text());
    });
    QObject::connect(runCommand,    &QLineEdit::textChanged  , [this](){
        model.setRunCommand(runCommand->text());
    });
    QObject::connect(cmakeSelectorOpen,    &QPushButton::clicked  , [this](){
        QString newCMakeFile = QFileDialog::getOpenFileName(this, "Select CMake File",
                                                        model.cmakePath(),
                                                        "CMake files (CMakeLists.txt)");
        if (!newCMakeFile.isEmpty()) model.setCmakePath(newCMakeFile);
        cmakeSelectorOpen->setText(model.cmakePath());

    });
    QObject::connect(isUseCmakePath,&QCheckBox::checkStateChanged , [this](){
        model.setisUseCmakePath((isUseCmakePath->checkState() == Qt::Checked));
        switchProjectInputMethod();
    });

    v_layout->addWidget(labels[priority] = new QLabel("priority", this));
    v_layout->addWidget(priority);
    v_layout->addWidget(labels[name] = new QLabel("name", this));
    v_layout->addWidget(name);
    v_layout->addWidget(labels[isUseCmakePath] = new QLabel("use cmake file as input", this));
    v_layout->addWidget(isUseCmakePath);
    v_layout->addWidget(labels[cmakeSelectorOpen] = new QLabel("Select cmake file", this));
    v_layout->addWidget(cmakeSelectorOpen);
    v_layout->addWidget(labels[buildCommand] = new QLabel("build Command", this));
    v_layout->addWidget(buildCommand);
    v_layout->addWidget(labels[runCommand] = new QLabel("run Command", this));
    v_layout->addWidget(runCommand);


    setLayout(v_layout);

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
