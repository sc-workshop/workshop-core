# Project: supercell-core

# C++ headers and source files
file(GLOB_RECURSE HEADERS source/*.h)
file(GLOB_RECURSE SOURCES source/*.cpp)

# choosing build type
if(SC_BUILD_SHARED)
    # shared library
    set(TARGET supercell-core-shared)
    add_library(${TARGET} SHARED)

    message(STATUS "Enabled SupercellSDK shared build")
else()
    # static library
    set(TARGET supercell-core-static)
    add_library(${TARGET} STATIC)
endif()

add_library(supercell::core ALIAS ${TARGET})
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "SupercellCore")

# add *.h and *.cpp files
target_sources(${TARGET} PRIVATE ${HEADERS} ${SOURCES})
get_target_property(SOURCE_DIR ${TARGET} SOURCE_DIR)
source_group(TREE ${SOURCE_DIR}/source FILES ${HEADERS} ${SOURCES})

# include directories
target_include_directories(${TARGET} PUBLIC source)

# setup project
sc_project_setup(${TARGET})
