#pragma once

#include <QMainWindow>
#include <memory>
#include "customscene.h"
#include "items/actionsblocker.h"
#include "qdatetimeedit.h"
#include "qpushbutton.h"
#include "services/runservice.h"

class QGraphicsView;
class QToolBar;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:

    void background_Layout();
    void saveLayout();
    void loadLayout();
    void setupToolBar();
    void setupRunService();
    void onRunStart();
    void onRunEnd();

    std::unique_ptr<CustomScene> m_scene;
    QGraphicsView* m_view;  // Owned by QMainWindow
    QToolBar* m_toolBar;  // ToolBar for draggable items
    ActionsBlocker* m_toolbar_blocker;
    ActionsBlocker* m_scene_blocker;
    QPushButton *startBtn;
    QPushButton *stopBtn ;
    QTimeEdit *timer;
    RunService runService;
};
