target_link_libraries(MainComputer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

target_sources(MainComputer PRIVATE
    ConditionsEditor.cpp
    SingleCondition.cpp
    ThenWidgetsLayout.cpp
    ThenGroupBox.cpp
    ConditionsGroup.cpp
    Window.cpp
    ExplorerBox.cpp
)