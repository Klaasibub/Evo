cmake_minimum_required(VERSION 3.5)

project(Evo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES
    main.cpp
    mainmenu.cpp
    Millionaire/millionaire.cpp
    Mosaic/mosaic.cpp
    Hangman/hangman.cpp
    Memory/memory.cpp
    utils.cpp
)
set(HEADERS
    mainmenu.h
    Millionaire/millionaire.h
    Mosaic/mosaic.h
    Hangman/hangman.h
    Memory/memory.h
    utils.h
)
set(UI
    mainmenu.ui
    Millionaire/millionaire.ui
    Mosaic/mosaic.ui
    Hangman/hangman.ui
    Memory/memory.ui
)
if(ANDROID)
  add_library(Evo SHARED ${SOURCES} ${HEADERS} ${UI})
else()
  add_executable(Evo ${SOURCES} ${HEADERS} ${UI})
endif()
target_link_libraries(Evo PRIVATE Qt5::Widgets)
