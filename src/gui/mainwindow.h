#pragma once

#include <QMainWindow>
#include <memory>
#include "customscene.h"

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

    void onConnectionStatusChanged(bool connected);

    CustomScene* m_scene;
    QGraphicsView* m_view;  // Owned by QMainWindow
    QToolBar* m_toolBar;  // ToolBar for draggable items
};
