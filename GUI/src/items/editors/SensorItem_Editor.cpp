#include "./SensorItem_Editor.h"
#include "CMakeUtils/getBuildAndRunCommands.h"
#include <GlobalState.h>
#include <QMessageBox>
#include <vector>

void SensorItem::Editor::initPriority()
{
    priority->setText(model.priority());
    layout->addWidget(labels[priority] = new QLabel("priority", this));
    layout->addWidget(priority);
}

void SensorItem::Editor::initName()
{
    name->setText(model.name());
    layout->addWidget(labels[name] = new QLabel("name", this));
    layout->addWidget(name);
}

void SensorItem::Editor::initBuildCommand()
{
    buildCommand->setText(model.buildCommand());
    layout->addWidget(labels[buildCommand] = new QLabel("build Command", this));
    layout->addWidget(buildCommand);
}

void SensorItem::Editor::initRunCommand()
{
    runCommand->setText(model.runCommand());
    layout->addWidget(labels[runCommand] = new QLabel("run Command", this));
    layout->addWidget(runCommand);
}

void SensorItem::Editor::initCmakeSelectorOpen()
{
    cmakeSelectorOpen->setText(cmakePath);
    QObject::connect(cmakeSelectorOpen,    &QPushButton::clicked  , [this](){
        QString newCMakeFile = QFileDialog::getOpenFileName(this, "Select CMake File",
                                                            cmakePath,
                                                            "CMake files (CMakeLists.txt)");
        if (!newCMakeFile.isEmpty()) cmakePath = newCMakeFile;
        cmakeSelectorOpen->setText(cmakePath);
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

void SensorItem::Editor::initSaveCancelBtns()
{
    QHBoxLayout *row = new QHBoxLayout;
    QPushButton* save = new QPushButton("Save", this);
    QPushButton* cancel = new QPushButton("Cancel", this);

    QObject::connect(save, &QPushButton::clicked, this, &Editor::onSaveBtnClicked);
    QObject::connect(cancel, &QPushButton::clicked, this, &Editor::onCancelBtnCliked);

    row->addWidget(save);
    row->addWidget(cancel);
    layout->addLayout(row);
}

void SensorItem::Editor::initParameters()
{
    initPriority();
    initName();
    initIsUseCmakePath();
    initBuildCommand();
    initRunCommand();
    initCmakeSelectorOpen();
}

void SensorItem::Editor::initLayout()
{
    setLayout(layout);
}

void SensorItem::Editor::onSaveBtnClicked()
{
    if(!(model.ownerID() == GlobalState::getInstance().myClientId()))
    {
        QMessageBox::warning(this, "Error", "Only the owner can modify the sensor");
        return;
    }
    auto itemData = model.serialize();

    itemData["priority"] = priority->text();
    itemData["name"] = name->text();
    itemData["buildCommand"] = buildCommand->text();
    itemData["runCommand"] = runCommand->text();
    itemData["cmakePath"] = cmakePath;
    itemData["isUseCmakePath"] = isUseCmakePath->checkState() == Qt::Checked;

    if (isUseCmakePath->checkState() == Qt::Checked)
    {
        auto commands = CMakeUtils::getBuildAndRunCommands(cmakePath);
        itemData["buildCommand"] = commands.first;
        itemData["runCommand"] = commands.second;
    }

    model.deserialize(itemData);
    model.notifyItemModified();
    onCancelBtnCliked();
}

void SensorItem::Editor::onCancelBtnCliked()
{
    GlobalState::getInstance().setCurrentSensorModel(nullptr);

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

SensorItem::Editor::Editor(SensorModel *_model) : model(*_model), cmakePath(model.cmakePath())
{
    initSaveCancelBtns();
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
    GlobalState::getInstance().log("closing...", "Terminal");
}
