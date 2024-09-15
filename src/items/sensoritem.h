#pragma once

#include "baseitem.h"
#include "sensormodel.h"

class PopupDialog;

class SensorItem : public BaseItem {
public:
    SensorItem(SensorModel* model, QGraphicsItem* parent = nullptr);

    PopupDialog* popupDialog = nullptr;


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool isInitialized() const;
    bool isExludeFromProject() const;

public slots:
    void onModelUpdated();

    virtual void openEditor();
    class Editor;
private:
    SensorModel* m_model;

    void setupUpdateButtonProxy();
    void setupCheckBoxProxy();

    void showButtons();
    void hideButtons();

    QGraphicsProxyWidget* m_updateProxy;
    QGraphicsProxyWidget* m_checkBoxProxy;

    QColor m_disabledColor = QColor(192, 192, 192); // Gray for disabled
    QColor m_excludedColor = QColor(250, 165, 142); // Red for excluded
    QColor m_availableColor = QColor(160, 253, 143); // Green for available

    void updateColor();    
};
