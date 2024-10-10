add_subdirectory(runservice)

set(SERVICES_SOURCES
    qtlogger.h
    qtlogger.cpp
)
set(SOURCES ${SOURCES} ${SERVICES_SOURCES} PARENT_SCOPE)
