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

set_target_properties(${TARGET} PROPERTIES
    FOLDER SupercellSDK/core
)

# add *.h and *.cpp files
target_sources(${TARGET} PRIVATE ${HEADERS} ${SOURCES})

# include directories
target_include_directories(${TARGET} PUBLIC source)

if (SC_CLI_DEPENDENCIES)
    include(cmake/argparse.cmake)
    target_link_libraries(${TARGET} PUBLIC argparse)
    
endif()

if (SC_SERIALIZATION_DEPENDENCIES)
    include(cmake/flatbuffers.cmake)
    target_link_libraries(${TARGET} PUBLIC FlatBuffers::FlatBuffers)

    include(cmake/json.cmake)
    target_link_libraries(${TARGET} PUBLIC nlohmann_json::nlohmann_json)
endif()

# setup project
sc_project_setup(${TARGET})
