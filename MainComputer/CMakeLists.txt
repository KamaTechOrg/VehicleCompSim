cmake_minimum_required(VERSION 3.16)
project(MainComputer)

#set for your version
set(Qt6_DIR "C:/Qt/6.7.3/msvc20022_64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

add_library(MainComputer
    src/maincomputer.h
    src/maincomputer.cpp
)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(GUI/src)
add_subdirectory(GUI/include)
add_subdirectory(tests)


set_target_properties(MainComputer PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

install(TARGETS MainComputer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MainComputer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

target_include_directories(MainComputer 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/GUI/include
)

target_link_libraries(MainComputer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    MainComputerHeaders  
    GuiHeaders
)
