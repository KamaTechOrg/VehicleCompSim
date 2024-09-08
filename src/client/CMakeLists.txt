# List source files in this directory
set(CLIENT_SOURCES
    websocketclient.cpp
    websocketclient.h
    main_client.cpp
    IActionHandler.h
    IdentifyHandler.h
    IdentifyHandler.cpp
    AddItemHandler.h
    AddItemHandler.cpp
    DeleteItemHandler.h
    DeleteItemHandler.cpp
    ModifyItemHandler.h
    ModifyItemHandler.cpp
    RunHandler.h
    RunHandler.cpp
    observer.h
    observerconfig.h
    observerconfig.cpp
)

# Add these sources to the parent scope
set(SOURCES ${SOURCES} ${CLIENT_SOURCES} PARENT_SCOPE)