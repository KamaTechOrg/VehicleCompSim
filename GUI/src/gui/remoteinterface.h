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
#include "globalconstants.h"

using namespace globalConstants;

class RemoteInterface : public QToolBar
{
    Q_OBJECT

public:
    explicit RemoteInterface(QWidget *parent = nullptr);

private slots:
    void onProjectItemClicked(const QModelIndex &index);
    void onRemoteModeChanged(bool isRemoteMode);
    void onConnectionStatusChanged(ConnectionState state);
    void onProjectAdded(ProjectModel* project);
    void onAddProjectClicked();
    void onCurrentProjectPublished(ProjectModel* project);
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

    QColor m_publishedColor = QColor(160, 253, 143); // Green
    QColor m_nonublishedColor = QColor(250, 165, 142); // Red
};