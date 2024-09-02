cmake_minimum_required(VERSION 3.16)

project(MainComputerHeaders LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(HEADER_FILES
    constants.h
    SimpleCondition.h
    CompositeCondition.h
    conditionsManager.h
    ConditionBase.h
    OrCondition.h
    AndCondition.h
    BinaryTreeBuilder.h
    ContainsCondition.h
    EndsWithCondition.h
    EqualsToCondition.h
    GreaterThanCondition.h
    SmallerThanCondition.h
    StartsWithCondition.h
    ConditionsFactory.h
    ConditionsManager.h
    Gui.h
    Communication.h

)

add_library(MainComputerHeaders INTERFACE)
target_sources(MainComputerHeaders INTERFACE ${HEADER_FILES})
target_include_directories(MainComputerHeaders INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/resources)


install(FILES ${HEADER_FILES} DESTINATION include)
