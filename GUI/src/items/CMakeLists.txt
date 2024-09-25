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
    qemusensoritem.h
    qemusensoritem.cpp
    customellipseItem.h
    parser.cpp
    parser.h
)

set(SOURCES ${SOURCES} ${ITEMS_SOURCES} PARENT_SCOPE)
