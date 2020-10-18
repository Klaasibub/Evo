cmake_minimum_required(VERSION 3.5)

project(Evo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

set(SOURCES
    main.cpp
    mainmenu.cpp
    Quiz/quiz.cpp
    Mosaic/mosaic.cpp
    Hangman/hangman.cpp
    Memory/memory.cpp
    utils.cpp
)
set(HEADERS
    mainmenu.h
    Quiz/quiz.h
    Mosaic/mosaic.h
    Hangman/hangman.h
    Memory/memory.h
    utils.h
    json_schemas.h
)
set(UI
    mainmenu.ui
    Quiz/quiz.ui
    Mosaic/mosaic.ui
    Hangman/hangman.ui
    Memory/memory.ui
)
qt5_add_resources(SOURCES resources.qrc)

if(ENABLE_UNIT_TESTS)
    add_library(Evo STATIC ${SOURCES} ${HEADERS} ${UI})
else()
    if(ANDROID)
        add_library(Evo SHARED ${SOURCES} ${HEADERS} ${UI})
    else()
        add_executable(Evo WIN32 ${SOURCES} ${HEADERS} ${UI})
    endif()
endif()


target_link_libraries(Evo PRIVATE Qt5::Widgets Qt5::Network)
