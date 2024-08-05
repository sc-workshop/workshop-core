# supercell-core


# C++ headers and source files
file(GLOB_RECURSE HEADERS source/*.h)
file(GLOB_RECURSE SOURCES source/*.cpp)

# choosing build type
if(SC_CORE_BUILD_SHARED)
    # shared library
    set(TARGET supercell-core-shared)

    add_library(${TARGET} SHARED)
    add_library(supercell::core ALIAS ${TARGET})

else()
    # static library
    set(TARGET supercell-core-static)

    add_library(${TARGET} STATIC)
    add_library(supercell::core ALIAS ${TARGET})

endif()

# add *.h and *.cpp files
target_sources(${TARGET} PRIVATE ${HEADERS} ${SOURCES})
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

# include directories
target_include_directories(${TARGET} PUBLIC source)

# setup project
sc_project_setup(${TARGET})
