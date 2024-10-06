#pragma once

#include <QToolTip>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>

#include "baseitem.h"
#include "sensormodel.h"
#include "PersistentTootip.h"
#include "CustomInfoWindow.h"
#include "widgets/verticalindicator.h"

class PopupDialog;

class SensorItem : public BaseItem {
public:
    SensorItem(SensorModel* model, QGraphicsItem* parent = nullptr);
    ~SensorItem();
    virtual SensorModel& getModel() const;
    VerticalIndicator* getVerticalIndicator() const; 

    QList<QString> columnNames;

    bool isInitialized() const;
    bool isExludeFromProject() const;
    void confirmRemove() override;
    void update_new_data(QList<QPair<QString, QString>> data);
    class Editor;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;


private slots:
    void onModelUpdated();
    void showInfoWindow();
    void onCustomWindowClosed();
    void onIsRunningChanged(bool isRunning);
    void setCloudIcon();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setupCheckBoxProxy();
    void setupIconLabelProxy();
    void showButtons();
    void hideButtons();
    QString fetchDataInTable();
    void updateInfoWindow();
    void updateColor();

    SensorModel* m_model;
    GlobalState &m_globalState;

    QGraphicsProxyWidget* m_checkBoxProxy;
    QGraphicsProxyWidget* m_verticalIndicatorProxy;
    VerticalIndicator* m_verticalIndicator;
    QLabel* m_cloudIcon;
    QColor m_disabledColor = QColor(192, 192, 192); // Gray for disabled
    QColor m_excludedColor = QColor(250, 165, 142); // Red for excluded
    QColor m_availableColor = QColor(160, 253, 143); // Green for available
    QTimer* m_updateWindowTimer;
    QGraphicsProxyWidget* m_infoWindowProxy = nullptr;
    CustomInfoWindow* m_infoWindow = nullptr;

    QList<QList<QPair<QString, QString>>>  all_data_final;
    QList<QPair<QString, QString>>  last_data_final;

    QList<QVariant> last_data;
    QList<QVariant> all_data;
    PersistentTooltip* m_persistentTooltip = nullptr;
    bool m_isOwnedByMe = false;
    bool mouse_pressed = false;
};
