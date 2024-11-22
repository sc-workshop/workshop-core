cmake_minimum_required(VERSION 3.22)

# workspace
project(workshop-sdk)

# options
option(WK_BUILD_SHARED "Build Workshop SDK as shared libraries" OFF)
option(WK_IN_SOURCE_BUILD "Build Workshop SDK with In-Source way" OFF)

# third-party
option(WK_CLI_DEPENDENCIES "Include libraries for CLI e.g Argparse" OFF)

option(WK_SERIALIZATION_JSON "Include JSON library" OFF)
option(WK_SERIALIZATION_FLATBUFFER "Include Flatbuffers library" OFF)

# includes
include(FetchContent)
include(cmake/project-macros.cmake)

# projects
add_subdirectory(workshop-core)
