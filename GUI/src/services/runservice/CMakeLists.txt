# Define the source files for runservice
set(RUNSERVICE_SOURCES
    runservice.cpp
    runner.h
    runservice.h    runservice.cpp
    runmanager.h    runmanager.cpp
    sensorrunner.h   sensorrunner.cpp
    IpFileHanler.h    IpFileHanler.cpp
    runvalidator.h   runvalidator.cpp
    maincomputerrunner.h maincomputerrunner.cpp
)

set(SOURCES ${SOURCES} ${RUNSERVICE_SOURCES} PARENT_SCOPE)

