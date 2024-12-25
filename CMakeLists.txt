cmake_minimum_required(VERSION 3.22)

# workspace
project(workshop-sdk)

# options
option(WK_BUILD_SHARED "Build Workshop SDK as shared libraries" OFF)
option(WK_IN_SOURCE_BUILD "Build Workshop SDK with In-Source way" OFF)

# includes
include(FetchContent)
include(cmake/project-macros.cmake)

# projects
add_subdirectory(workshop-core)
