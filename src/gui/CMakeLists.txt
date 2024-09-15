set(CLIENT_SOURCES
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    customwidget.cpp
    customwidget.h
    customscene.cpp
    customscene.h
    LiveUpdate.cpp
    LogReader.cpp
    SimulationRecorder.cpp
    SimulationReplayer.cpp
    SimulationControlPanel.h
    SimulationControlPanel.cpp
    popupdialog.cpp
    popupdialog.h
    popupdialog.ui
    editpanel.h
    editpanel.cpp
#    DB_handler.h
#    DB_handler.cpp
)

set(SOURCES ${SOURCES} ${CLIENT_SOURCES} PARENT_SCOPE)
