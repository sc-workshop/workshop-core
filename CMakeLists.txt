cmake_minimum_required(VERSION 3.22)

# workspace
project(supercell-sdk)

# options
option(SC_BUILD_SHARED "Build Supercell SDK as shared libraries" OFF)
option(SC_IN_SOURCE_BUILD "Build Supercell SDK with In-Source way" OFF)

# third-party
option(SC_CLI_DEPENDENCIES "Include libraries for CLI e.g Argparse" OFF)
option(SC_SERIALIZATION_DEPENDENCIES "Include libraries for serialization e.g JSON or Flatbuffers")

# includes
include(FetchContent)
include(cmake/supercell-sdk-macros.cmake)

# projects
add_subdirectory(supercell-core)
