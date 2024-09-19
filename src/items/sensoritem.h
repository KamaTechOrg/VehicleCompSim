#pragma once

#include <QToolTip>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "baseitem.h"
#include "sensormodel.h"
#include "gui/PersistentTootip.h"
#include "gui/CustomInfoWindow.h"

class PopupDialog;

class SensorItem : public BaseItem {
public:
    SensorItem(SensorModel* model, QGraphicsItem* parent = nullptr);
    ~SensorItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool isInitialized() const;
    bool isExludeFromProject() const;
    virtual SensorModel& getModel();
    void select();
    class Editor;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

public:
    QList<QString> columnNames;

private:
    void setupUpdateButtonProxy();
    void setupCheckBoxProxy();
    void showButtons();
    void hideButtons();
    void showInfoWindow();
    QString fetchDataInTable();
    void updateInfoWindow();
    void updateColor();

    SensorModel* m_model;
    QGraphicsProxyWidget* m_updateProxy;
    QGraphicsProxyWidget* m_checkBoxProxy;
    QColor m_disabledColor = QColor(192, 192, 192); // Gray for disabled
    QColor m_excludedColor = QColor(250, 165, 142); // Red for excluded
    QColor m_availableColor = QColor(160, 253, 143); // Green for available
    QTimer* m_updateWindowTimer;
    QGraphicsProxyWidget* m_infoWindowProxy = nullptr;
    CustomInfoWindow* m_infoWindow = nullptr;
    QList<QVariant> last_data;
    QList<QVariant> all_data;
    PersistentTooltip* m_persistentTooltip = nullptr;
    bool mouse_pressed = false;
    bool playMode = false;


public slots:
    void onModelUpdated();

private slots:
    void update_data(const QString& sensorId, QList<QVariant> data);
    void update_column_names(const QString& sensorId, QList<QString> data);

    void onCustomWindowClosed();

};
