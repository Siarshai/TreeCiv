cmake_minimum_required(VERSION 3.14)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_PREFIX_PATH "/home/siarshai/Programming/Qt5.12.11/5.12.11/gcc_64/lib/cmake")

set(PROJECT_NAME tree_civilization)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -Wall -Werror -std=c++11 -fstrict-aliasing -pedantic-errors -pedantic -Wno-deprecated-declarations -Wno-unused-variable")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

set(TREE_ITEMS
        src/TreeParts/Interfaces/TreeNode.cpp
        src/TreeParts/Interfaces/TreeNode.h
        src/TreeParts/ConcreteNodes/TrunkNode.cpp
        src/TreeParts/ConcreteNodes/TrunkNode.h
        src/TreeParts/ConcreteNodes/BranchNode.cpp
        src/TreeParts/ConcreteNodes/BranchNode.h
        src/TreeParts/ConcreteNodes/ResourceNode.cpp
        src/TreeParts/ConcreteNodes/ResourceNode.h
        src/TreeParts/ConcreteNodes/NestNode.cpp
        src/TreeParts/ConcreteNodes/NestNode.h
        src/TreeParts/TreeParser.cpp
        src/TreeParts/TreeParser.h)

set(MAIN_SOURCES
        src/main.cpp
        src/MapTreeModel.cpp
        src/MapTreeModel.h
        ${TREE_ITEMS})
qt5_add_resources(QT_RESOURCES src/qml.qrc)

add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${QT_RESOURCES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt5::Core Qt5::Widgets Qt5::Quick)


add_executable(${PROJECT_NAME}_test
        src/main_test_all.cpp
        src/TreeParts/TreeParser_test.cpp
        ${TREE_ITEMS})

target_link_libraries(
        ${PROJECT_NAME}_test
        Qt5::Core gtest gtest_main gmock pthread)
