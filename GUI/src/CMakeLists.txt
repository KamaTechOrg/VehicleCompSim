# Collect all source files
file(GLOB_RECURSE SOURCES
    "*.cpp"
    "*.h"
    "*.ui"
)

# Create a library target for all source files
add_library(VehicleCompSimCore STATIC ${SOURCES} )

# Set include directories
target_include_directories(VehicleCompSimCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/client
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/items
    ${CMAKE_CURRENT_SOURCE_DIR}/items/editors
    ${CMAKE_CURRENT_SOURCE_DIR}/services
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/state
    ${CMAKE_SOURCE_DIR}/GUI/include/VehicleCompSim/utils
    ${CMAKE_SOURCE_DIR}/GUI/resources/icons
    ${CMAKE_SOURCE_DIR}/MainComputer/include
    ${CMAKE_SOURCE_DIR}/Communication/server/include
    ${CMAKE_SOURCE_DIR}/Communication/User_Directory/client
    ${CMAKE_SOURCE_DIR}/Communication/utilities
)

# Link Qt libraries
target_link_libraries(VehicleCompSimCore PUBLIC
    Qt::Widgets
    Qt::WebSockets
    Qt::Sql
    MainComputer
    server_lib
    client_lib
    services
)

# Add subdirectories
add_subdirectory(client)
add_subdirectory(gui)
add_subdirectory(items)
add_subdirectory(items/editors)
add_subdirectory(services)
add_subdirectory(models)
add_subdirectory(state)
