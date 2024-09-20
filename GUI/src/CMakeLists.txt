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
    ${CMAKE_CURRENT_SOURCE_DIR}/services
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/state
    ${CMAKE_SOURCE_DIR}/GUI/include/VehicleCompSim/utils
)

# Link Qt libraries
target_link_libraries(VehicleCompSimCore PUBLIC
    Qt::Widgets
    Qt::WebSockets
    Qt::Sql
)

# Add subdirectories
add_subdirectory(client)
add_subdirectory(gui)
add_subdirectory(items)
add_subdirectory(services)
add_subdirectory(models)
add_subdirectory(state)
