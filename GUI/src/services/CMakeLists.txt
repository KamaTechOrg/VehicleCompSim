add_subdirectory(runservice)

set(SERVICES_SOURCES
    qtlogger.h
    qtlogger.cpp
    SimulationRecorder.h
    SimulationRecorder.cpp
    SimulationReplayer.h
    SimulationReplayer.cpp
    DB_handler.h
    DB_handler.cpp
    saveAndLoad.cpp
    saveAndLoad.h
    parser.cpp
    parser.h
)
set(SOURCES ${SOURCES} ${SERVICES_SOURCES} PARENT_SCOPE)
