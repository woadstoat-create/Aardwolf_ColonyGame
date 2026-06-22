cmake_minimum_required(VERSION 3.28)

project(
    ColonySim
    VERSION 0.1.0
    DESCRIPTION "A high-performance multi-layer colony simulation prototype"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Keep project build products in predictable locations.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(CTest)
include(FetchContent)
include(cmake/ProjectOptions.cmake)

# Build dependencies statically for now. This avoids runtime DLL deployment
# during the early project milestones.
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Only build the SFML modules currently required by the renderer.
set(SFML_BUILD_AUDIO OFF CACHE BOOL "Do not build SFML Audio" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "Do not build SFML Network" FORCE)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(sfml)

if(BUILD_TESTING)
    FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.15.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
    )

    FetchContent_MakeAvailable(catch2)

    # Required when Catch2 is acquired through FetchContent.
    list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
endif()

add_subdirectory(src/core)
add_subdirectory(src/app)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()