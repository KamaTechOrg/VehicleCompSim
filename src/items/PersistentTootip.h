//
// Created by OWNER on 10/09/2024.
//

#ifndef VEHICLECOMPSIM_PERSISTENTTOOTIP_H
#define VEHICLECOMPSIM_PERSISTENTTOOTIP_H

#include <QGraphicsSceneHoverEvent>
#include <QGraphicsItem>
#include <QColor>
#include <QPointF>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
class PersistentTooltip : public QWidget {
public:
    PersistentTooltip(QWidget* parent = nullptr) : QWidget(parent, Qt::ToolTip | Qt::FramelessWindowHint) {
        label = new QLabel(this);
        label->setTextFormat(Qt::RichText);
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(label);
        setLayout(layout);
    }

    void setText(const QString& text) {
        label->setText(text);
    }

private:
    QLabel* label;
};



#endif //VEHICLECOMPSIM_PERSISTENTTOOTIP_H
