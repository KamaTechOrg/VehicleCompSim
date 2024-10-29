#pragma once

#include <QToolTip>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QTextEdit>

#include "baseitem.h"
#include "../models/sensorModel.h"
#include "PersistentTootip.h"
#include "widgets/verticalindicator.h"

class PopupDialog;

class SensorItem : public BaseItem {
public:
    SensorItem(SensorModel* model, QGraphicsItem* parent = nullptr);
    ~SensorItem();
    virtual SensorModel& getModel() const;
    VerticalIndicator* getVerticalIndicator() const;
    bool isInitialized() const;
    bool isExludeFromProject() const;
    void confirmRemove() override;
    void update_new_data(const QString & data);
    class Editor;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private slots:
    void onModelUpdated();
    void onIsRunningChanged(bool isRunning);
    void setCloudIcon();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setupCheckBoxProxy();
    void setupIconLabelProxy();
    void showButtons();
    void hideButtons();
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
    QString last_data;
    PersistentTooltip* m_persistentTooltip = nullptr;
    bool m_isOwnedByMe = false;
    bool programRunning = false;
};
