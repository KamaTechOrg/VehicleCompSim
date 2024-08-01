cmake_minimum_required(VERSION 3.16)
project(MainComputer LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_executable(MainComputer
    src/main.cpp
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

target_include_directories(MainComputer PRIVATE ${CMAKE_SOURCE_DIR}/resources)


target_link_libraries(MainComputer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    MainComputerHeaders  
    GuiHeaders
)