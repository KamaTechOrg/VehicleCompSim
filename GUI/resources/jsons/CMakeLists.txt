# Collect all JSON files in the jsons directory
file(GLOB JSONS "${CMAKE_CURRENT_SOURCE_DIR}/*.json")

# Define the destination directory in the build folder
set(JSONS_DEST_DIR "${CMAKE_BINARY_DIR}/GUI/resources/jsons")

# Create the destination directory
file(MAKE_DIRECTORY ${JSONS_DEST_DIR})

# Copy each JSON file to the destination directory
foreach(JSON ${JSONS})
    get_filename_component(JSON_NAME ${JSON} NAME)
    configure_file(${JSON} ${JSONS_DEST_DIR}/${JSON_NAME} COPYONLY)
endforeach()