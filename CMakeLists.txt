cmake_minimum_required(VERSION 3.22)

# workspace
project(supercell-sdk)

# options
option(SC_BUILD_SHARED "Build Supercell SDK as shared libraries" OFF)

# includes
include(FetchContent)
include(cmake/supercell-sdk-macros.cmake)

# projects
add_subdirectory(supercell-core)
