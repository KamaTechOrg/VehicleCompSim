#include "RemoteInterface.h"
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
#include <QCheckBox>

#include "projectmodel.h"


RemoteInterface::RemoteInterface(QWidget *parent)
    : QToolBar(parent), m_globalState(GlobalState::getInstance())
{
    setupUI();
    populateProjectList();

    // Connect to GlobalState signals
    connect(&m_globalState, &GlobalState::isRemoteModeChanged, this, &RemoteInterface::onRemoteModeChanged);
    connect(&m_globalState, &GlobalState::connectionStateChanged, this, &RemoteInterface::onConnectionStatusChanged);
    connect(&m_globalState, &GlobalState::projectAdded, this, &RemoteInterface::onProjectAdded);
    connect(&m_globalState, &GlobalState::currentProjectPublished, this, &RemoteInterface::onCurrentProjectPublished);

    // Initial update
    onRemoteModeChanged(m_globalState.isRemoteMode());
    onConnectionStatusChanged(m_globalState.connectionState());
}

void RemoteInterface::setupUI()
{
    // Mode switch button
    m_switchModeButton = new IconButton(globalConstants::Icons::ToggleSwitchOff, "Switch to Remote Mode", this);
    connect(m_switchModeButton, &QPushButton::clicked, this, [this]() {
        bool isRemoteMode = !m_globalState.isRemoteMode();
        m_globalState.setIsRemoteMode(isRemoteMode);
    });

    //add check box for enable test mode
    QCheckBox* testModeCheckBox = new QCheckBox("Enable Test Mode", this);
    connect(testModeCheckBox, &QCheckBox::stateChanged, this, [this](int state) {
        bool isTestMode = state == Qt::Checked;
        m_globalState.setIsTest(isTestMode);
    });


    // Add project button
    m_addProjectButton = new IconButton(globalConstants::Icons::AddProject, "Add Project", this);
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
    m_scrollLeftButton = new IconButton(globalConstants::Icons::ArrowLeft, "Scroll Left", this);
    m_scrollLeftButton->setCustomSize(QSize(25, 25));
    m_scrollLeftButton->setIconSize(QSize(25, 25));
    connect(m_scrollLeftButton, &QPushButton::clicked, this, &RemoteInterface::scrollLeft);

    m_scrollRightButton = new IconButton(globalConstants::Icons::ArrowRight, "Scroll Right", this);
    m_scrollRightButton->setCustomSize(QSize(25, 25));
    m_scrollRightButton->setIconSize(QSize(25, 25));
    connect(m_scrollRightButton, &QPushButton::clicked, this, &RemoteInterface::scrollRight);

    // Connection status group box
    m_connectionGroupBox = new QGroupBox(this);
    QVBoxLayout *connectionLayout = new QVBoxLayout(m_connectionGroupBox);
    connectionLayout->addWidget(m_switchModeButton);
    connectionLayout->addWidget(testModeCheckBox);
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
        if(project->isPublished() &&  m_globalState.connectionState() != ConnectionState::Online){
            continue;
        }
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

void RemoteInterface::onConnectionStatusChanged(ConnectionState state)
{
    QString statusText;
    QString color;

    switch (state) {
        case ConnectionState::Connecting:
            statusText = "Connecting...";
            color = "orange";
            m_switchModeButton->changeIcon(globalConstants::Icons::ToggleSwitchMiddle);
            break;
        case ConnectionState::Online:
            statusText = "Online";
            color = "green";
            m_switchModeButton->changeIcon(globalConstants::Icons::ToggleSwitch);
            break;
        case ConnectionState::Offline:
            statusText = "Offline";
            color = "red";
            if(m_globalState.isRemoteMode()){
                m_switchModeButton->changeIcon(globalConstants::Icons::ToggleSwitchMiddle);
            } else {
                m_switchModeButton->changeIcon(globalConstants::Icons::ToggleSwitchOff);
            }
            break;
    }

    m_connectionGroupBox->setTitle(statusText);
    m_connectionGroupBox->setStyleSheet(QString("QGroupBox { color: %1; }").arg(color));

    populateProjectList();
}

void RemoteInterface::onProjectItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QStandardItem *item = m_projectListModel->itemFromIndex(index);
        ProjectModel* project = m_projectItemMap.value(item, nullptr);
        if (project) {
            m_globalState.setCurrentProject(project);
        }
    }
}

void RemoteInterface::onRemoteModeChanged(bool isRemoteMode)
{
    if (isRemoteMode) {
        m_switchModeButton->setToolTip("Switch to Local Mode");
        if(m_globalState.connectionState() == ConnectionState::Online) {
            m_switchModeButton->changeIcon(globalConstants::Icons::ToggleSwitch);
        } else {
            m_switchModeButton->changeIcon(globalConstants::Icons::ToggleSwitchMiddle);
        }
    } else {
        m_switchModeButton->setToolTip("Switch to Remote Mode");
        m_switchModeButton->changeIcon(globalConstants::Icons::ToggleSwitchOff);
    }
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