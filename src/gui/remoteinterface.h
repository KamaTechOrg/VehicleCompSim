#pragma once

#include <QWidget>
#include <QToolBar>
#include <QListView>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QMap>
#include <QStandardItemModel>
#include <QScrollArea>
#include <QScrollBar>
#include <QGroupBox>
#include "globalstate.h"
#include "projectmodel.h"

class RemoteInterface : public QToolBar
{
    Q_OBJECT

public:
    explicit RemoteInterface(QWidget *parent = nullptr);

private slots:
    void onProjectItemClicked(const QModelIndex &index);
    void onRemoteModeChanged(bool isRemoteMode);
    void onConnectionStatusChanged(bool isOnline);
    void onConnectingStatusChanged(bool isConnecting);
    void onProjectAdded(ProjectModel* project);
    void onAddProjectClicked();
    void scrollLeft();
    void scrollRight();

private:
    void setupUI();
    void populateProjectList();
    void updateConnectionStatus();

    QPushButton *m_switchModeButton;
    QPushButton *m_addProjectButton;
    QListView *m_projectListView;
    QGroupBox *m_connectionGroupBox;
    QLabel *m_connectionStatusLabel;
    QLabel *m_connectingStatusLabel;
    QTimer *m_connectionTimer;
    GlobalState &m_globalState;

    QStandardItemModel *m_projectListModel;
    QMap<QStandardItem*, ProjectModel*> m_projectItemMap;

    QScrollArea *m_scrollArea;
    QPushButton *m_scrollLeftButton;
    QPushButton *m_scrollRightButton;
};