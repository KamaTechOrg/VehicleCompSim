#include "./SensorItem_Editor.h"

SensorItem::Editor::Editor(SensorItem *_sensor) : sensor(_sensor), name(new QLineEdit(this)),
    save(new QPushButton("save", this))
{
    name->setText(sensor->getModel().name());
    QObject::connect(save, &QPushButton::clicked, [this](){
        sensor->getModel().setName(name->text());
        EditPanel::closeCurrentEditor();
    });
}

void SensorItem::Editor::open()
{
    this->show();
}

void SensorItem::Editor::close()
{
    qInfo() << "closing...";
}
