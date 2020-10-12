project(root_evo_project)
option(ENABLE_UNIT_TESTS "Enable unit tests." OFF)

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all")

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all")

SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single Directory for all")

if(ENABLE_UNIT_TESTS)
    add_subdirectory(tests tests)
    enable_testing()
endif()

add_subdirectory(src src)
