# Collect all SVG files in the icons directory
file(GLOB ICONS "${CMAKE_CURRENT_SOURCE_DIR}/*.svg")

# Define the destination directory in the build folder
set(ICONS_DEST_DIR "${CMAKE_BINARY_DIR}/GUI/resources/icons")

# Create the destination directory
file(MAKE_DIRECTORY ${ICONS_DEST_DIR})

# Copy each icon to the destination directory
foreach(ICON ${ICONS})
    get_filename_component(ICON_NAME ${ICON} NAME)
    configure_file(${ICON} ${ICONS_DEST_DIR}/${ICON_NAME} COPYONLY)
endforeach()

message(STATUS "Icons will be copied to: ${ICONS_DEST_DIR}")

# # Collect all SVG files in the icons directory
# file(GLOB ICONS "${CMAKE_CURRENT_SOURCE_DIR}/*.svg")

# # Create a temporary icons.qrc file
# set(QRC_FILE "${CMAKE_CURRENT_BINARY_DIR}/icons.qrc")
# file(WRITE ${QRC_FILE} "<RCC>\n<qresource prefix=\"/GUI/resources/icons\">\n")
# foreach(ICON ${ICONS})
#     get_filename_component(ICON_NAME ${ICON} NAME)
#     file(APPEND ${QRC_FILE} "    <file alias=\"${ICON_NAME}\">${ICON}</file>\n")
# endforeach()
# file(APPEND ${QRC_FILE} "</qresource>\n</RCC>\n")

# # Add the icons to the resource file
# qt_add_resources(ICONS_RESOURCES ${QRC_FILE})

# # this should be in the cmakelists of the src folder

# # Include the icons resources
# include(${CMAKE_CURRENT_SOURCE_DIR}/../resources/icons/CMakeLists.txt)

# # Add the icons resources to the library
# target_sources(VehicleCompSimCore PRIVATE ${ICONS_RESOURCES})