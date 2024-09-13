set(CLIENT_SOURCES
    serializableitem.cpp
    serializableitem.h
    baseitem.cpp
    baseitem.h
    sensoritem.cpp
    sensoritem.h
    connectoritem.cpp
    connectoritem.h
    edgeitem.cpp
    edgeitem.h
    actionsblocker.h
    actionsblocker.cpp
    PersistentTootip.cpp
    PersistentTootip.h
)

set(SOURCES ${SOURCES} ${CLIENT_SOURCES} PARENT_SCOPE)
