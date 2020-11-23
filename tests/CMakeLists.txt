cmake_minimum_required(VERSION 3.5)

project(UnitTests LANGUAGES CXX)


find_package(Qt5 COMPONENTS Widgets Network Multimedia Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_SOURCES
    main_gui_tests.cpp
    utils_tests.cpp
    quiz_tests.cpp
    memory_tests.cpp
    mosaic_tests.cpp
    hangman_tests.cpp
)

set(TEST_HEADERS
    main_gui_tests.h
    utils_tests.h
    quiz_tests.h
    memory_tests.h
    mosaic_tests.h
    hangman_tests.h
)

add_executable(UnitTests ${SOURCES} ${HEADERS} main.cpp ${TEST_SOURCES} ${TEST_HEADERS})

target_link_libraries(UnitTests LINK_PRIVATE Evo Qt5::Test Qt5::Widgets Qt5::Network Qt5::Multimedia)
