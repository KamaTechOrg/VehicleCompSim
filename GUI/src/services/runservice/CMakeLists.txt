# Define the source files for runservice
set(RUNSERVICE_SOURCES
    runservice.cpp
    runner.h
    runmanager.h runmanager.cpp
    sensorrunner.h sensorrunner.cpp
    IpFileHanler.cpp
    IpFileHanler.h
)

set(SOURCES ${SOURCES} ${RUNSERVICE_SOURCES} PARENT_SCOPE)

