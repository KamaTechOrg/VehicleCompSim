target_sources(MainComputer PRIVATE
    SimpleCondition.cpp
    CompositeCondition.cpp
    OrCondition.cpp
    AndCondition.cpp
    BinaryTreeBuilder.cpp
    ContainsCondition.cpp
    EndsWithCondition.cpp
    EqualsToCondition.cpp
    GreaterThanCondition.cpp
    SmallerThanCondition.cpp
    StartsWithCondition.cpp
    ConditionsFactory.cpp
    Gui.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/resources)
