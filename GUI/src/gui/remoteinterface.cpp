#include "RemoteInterface.h"
#include "projectmodel.h"
#include <QRandomGenerator>
#include <QInputDialog>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

RemoteInterface::RemoteInterface(QWidget *parent)
    : QToolBar(parent), m_globalState(GlobalState::getInstance())
{
    setupUI();
    populateProjectList();

    // Connect to GlobalState signals
    connect(&m_globalState, &GlobalState::isRemoteModeChanged, this, &RemoteInterface::onRemoteModeChanged);
    connect(&m_globalState, &GlobalState::isOnlineChanged, this, &RemoteInterface::onConnectionStatusChanged);
    connect(&m_globalState, &GlobalState::isConnectingChanged, this, &RemoteInterface::onConnectingStatusChanged);
    connect(&m_globalState, &GlobalState::projectAdded, this, &RemoteInterface::onProjectAdded);
    connect(&m_globalState, &GlobalState::currentProjectPublished, this, &RemoteInterface::onCurrentProjectPublished);

    // Initial update
    onRemoteModeChanged(m_globalState.isRemoteMode());
    onConnectionStatusChanged(m_globalState.isOnline());
    onConnectingStatusChanged(m_globalState.isConnecting());
}

void RemoteInterface::setupUI()
{
    // Mode switch button
    m_switchModeButton = new QPushButton("Switch to Remote Mode", this);
    connect(m_switchModeButton, &QPushButton::clicked, this, [this]() {
        bool isRemoteMode = !m_globalState.isRemoteMode();
        m_globalState.setIsRemoteMode(isRemoteMode);
    });

    // Add project button
    m_addProjectButton = new QPushButton("Add Project", this);
    connect(m_addProjectButton, &QPushButton::clicked, this, &RemoteInterface::onAddProjectClicked);

    // Project list view
    m_projectListView = new QListView(this);
    m_projectListModel = new QStandardItemModel(this);
    m_projectListView->setModel(m_projectListModel);
    // m_projectListView->setViewMode(QListView::IconMode);
    m_projectListView->setFlow(QListView::LeftToRight);
    m_projectListView->setWrapping(false);
    m_projectListView->setFixedHeight(50); 
    connect(m_projectListView, &QListView::clicked, this, &RemoteInterface::onProjectItemClicked);

    // Scroll area to contain the list view
    QWidget *scrollWidget = new QWidget(this);
    QHBoxLayout *scrollLayout = new QHBoxLayout(scrollWidget);
    scrollLayout->addWidget(m_projectListView);
    scrollWidget->setLayout(scrollLayout);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidget(scrollWidget);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Navigation buttons
    m_scrollLeftButton = new QPushButton("<", this);
    m_scrollLeftButton->setFixedSize(25, 25);
    connect(m_scrollLeftButton, &QPushButton::clicked, this, &RemoteInterface::scrollLeft);

    m_scrollRightButton = new QPushButton(">", this);
    m_scrollRightButton->setFixedSize(25, 25);
    connect(m_scrollRightButton, &QPushButton::clicked, this, &RemoteInterface::scrollRight);

    // Connection status group box
    m_connectionGroupBox = new QGroupBox(this);
    QVBoxLayout *connectionLayout = new QVBoxLayout(m_connectionGroupBox);
    connectionLayout->addWidget(m_switchModeButton);
    connectionLayout->addWidget(m_addProjectButton);
    connectionLayout->addWidget(m_scrollLeftButton);
    connectionLayout->addWidget(m_scrollArea);
    connectionLayout->addWidget(m_scrollRightButton);
    m_connectionGroupBox->setLayout(connectionLayout);
    // make group box content direction to be horizontal
    connectionLayout->setDirection(QBoxLayout::LeftToRight);

    // Add the group box to the toolbar
    addWidget(m_connectionGroupBox);
}

void RemoteInterface::scrollLeft()
{
    int value = m_scrollArea->horizontalScrollBar()->value();
    m_scrollArea->horizontalScrollBar()->setValue(value - 60); 
}

void RemoteInterface::scrollRight()
{
    int value = m_scrollArea->horizontalScrollBar()->value();
    m_scrollArea->horizontalScrollBar()->setValue(value + 60); 
}

void RemoteInterface::populateProjectList()
{
    m_projectListModel->clear();
    m_projectItemMap.clear(); // Clear the map

    for (const auto &project : m_globalState.projects()) {
        QStandardItem *item = new QStandardItem(project->name());
        item->setSizeHint(QSize(100, 50)); 
        item->setTextAlignment(Qt::AlignCenter);
        item->setEditable(false);

        // Set background color based on isPublished field
        if (project->isPublished()) {
            item->setBackground(m_publishedColor);
        } else {
            item->setBackground(m_nonublishedColor);
        }

        m_projectListModel->appendRow(item);
        m_projectItemMap[item] = project; 
    }
}

void RemoteInterface::updateConnectionStatus()
{
    QString statusText;
    QString color;

    if (m_globalState.isConnecting()) {
        statusText = "Connecting...";
        color = "orange";
    } else if (m_globalState.isOnline()) {
        statusText = "Online";
        color = "green";
    } else {
        statusText = "Offline";
        color = "red";
    }

    m_connectionGroupBox->setTitle(statusText);
    m_connectionGroupBox->setStyleSheet(QString("QGroupBox { color: %1; }").arg(color));
}

void RemoteInterface::onProjectItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QStandardItem *item = m_projectListModel->itemFromIndex(index);
        ProjectModel* project = m_projectItemMap.value(item, nullptr);
        if (project) {
            emit m_globalState.setCurrentProject(project);
        }
    }
}

void RemoteInterface::onRemoteModeChanged(bool isRemoteMode)
{
    if (isRemoteMode) {
        m_switchModeButton->setText("Switch to Local Mode");
    } else {
        m_switchModeButton->setText("Switch to Remote Mode");
    }
    updateConnectionStatus();
}

void RemoteInterface::onConnectionStatusChanged(bool isOnline)
{
    updateConnectionStatus();
}

void RemoteInterface::onConnectingStatusChanged(bool isConnecting)
{
    updateConnectionStatus();
}

void RemoteInterface::onProjectAdded(ProjectModel* project)
{
    if (project) {
        QStandardItem *item = new QStandardItem(project->name());
        item->setSizeHint(QSize(100, 50)); // Adjust the item size as needed
        item->setTextAlignment(Qt::AlignCenter);
        item->setEditable(false);

        // Set background color based on isPublished field
        if (project->isPublished()) {
            item->setBackground(m_publishedColor);
        } else {
            item->setBackground(m_nonublishedColor);
        }

        m_projectListModel->appendRow(item);
        m_projectItemMap[item] = project; // Add to the map
    }
}

void RemoteInterface::onCurrentProjectPublished(ProjectModel* project)
{
    populateProjectList();
}

void RemoteInterface::onAddProjectClicked()
{
    bool ok;
    QString projectName = QInputDialog::getText(this, tr("Add Project"),
                                                tr("Project Name:"), QLineEdit::Normal,
                                                "", &ok);
    if (ok && !projectName.isEmpty()) {
        ProjectModel* newProject = new ProjectModel(projectName);
        m_globalState.addProject(newProject);
    }
}