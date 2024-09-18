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
    DB_handler.h
    DB_handler.cpp
    buffer_test.h
    buffer_test.cpp
)

set(SOURCES ${SOURCES} ${CLIENT_SOURCES} PARENT_SCOPE)
