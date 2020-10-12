cmake_minimum_required(VERSION 3.5)

project(UnitTests LANGUAGES CXX)


find_package(Qt5 COMPONENTS Widgets Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

add_executable(UnitTests ${SOURCES} ${HEADERS} tst_unittests.cpp)

add_test(NAME UnitTests COMMAND UnitTests)

target_link_libraries(UnitTests LINK_PRIVATE Evo Qt5::Test Qt5::Widgets)
