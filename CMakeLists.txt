cmake_minimum_required(VERSION 3.21)
project(FEUP_AED_STCP)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)


add_executable(stcp
    main.cpp
    src/graphs/least_bus_changes.cpp
    src/graphs/least_distance.cpp
    src/graphs/least_money_spent.cpp
    src/graphs/least_stops.cpp
    src/dataset.cpp
    src/harversine.cpp
    src/interact.cpp
    src/names.cpp
    src/pathfinding.cpp
    src/ui.cpp
)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(BUILD_DOC_DIR "${CMAKE_SOURCE_DIR}/docs/output")
    if(NOT EXISTS "${BUILD_DOC_DIR}")
        file(MAKE_DIRECTORY "${BUILD_DOC_DIR}")
    endif()
    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    configure_file("${DOXYGEN_IN}" "${DOXYGEN_OUT}" @ONLY)
    message("Doxygen build started")
    add_custom_target(Doxygen ALL
            COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_OUT}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the documentation.")
endif(DOXYGEN_FOUND)

