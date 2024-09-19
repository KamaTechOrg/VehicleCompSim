# List source files in this directory
set(CLIENT_SOURCES
    websocketclient.cpp
    websocketclient.h
    observer.h
    observerconfig.h
    observerconfig.cpp
    clientconstants.h
    handlers/IActionHandler.h
    handlers/IdentifyHandler.h
    handlers/IdentifyHandler.cpp
    handlers/AddItemHandler.h
    handlers/AddItemHandler.cpp
    handlers/DeleteItemHandler.h
    handlers/DeleteItemHandler.cpp
    handlers/ModifyItemHandler.h
    handlers/ModifyItemHandler.cpp
    handlers/ProjectHandler.h
    handlers/ProjectHandler.cpp
    handlers/RunHandler.h
    handlers/RunHandler.cpp

)

# Add these sources to the parent scope
set(SOURCES ${SOURCES} ${CLIENT_SOURCES} PARENT_SCOPE)