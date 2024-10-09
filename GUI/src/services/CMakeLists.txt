add_subdirectory(runservice)

set(SERVICES_SOURCES
    logger.h
    logger.cpp
)
set(SOURCES ${SOURCES} ${SERVICES_SOURCES} PARENT_SCOPE)
