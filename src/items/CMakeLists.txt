set(ITEMS_SOURCES
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

    qemusensoritem.h
    qemusensoritem.cpp

)

set(SOURCES ${SOURCES} ${ITEMS_SOURCES} PARENT_SCOPE)
