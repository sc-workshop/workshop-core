# Project: workshop-core

# C++ headers and source files
file(GLOB_RECURSE HEADERS source/*.h source/*.hpp)
file(GLOB_RECURSE SOURCES source/*.cpp)

# choosing build type
if(WK_BUILD_SHARED)
    # shared library
    set(TARGET workshop-core-shared)
    add_library(${TARGET} SHARED)

    message(STATUS "Enabled WorkshopSDK shared build")
else()
    # static library
    set(TARGET workshop-core-static)
    add_library(${TARGET} STATIC)
endif()

add_library(wk::core ALIAS ${TARGET})
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "WorkshopCore")

set_target_properties(${TARGET} PROPERTIES
    FOLDER WorkshopSDK/core
)

# add *.h and *.cpp files
target_sources(${TARGET} PRIVATE ${HEADERS} ${SOURCES})

# include directories
target_include_directories(${TARGET} PUBLIC source)

if (WK_CLI_DEPENDENCIES)
    include(cmake/argparse.cmake)
    target_link_libraries(${TARGET} PUBLIC argparse)
    
endif()

if (WK_SERIALIZATION_FLATBUFFER)
    include(cmake/flatbuffers.cmake)
    include (cmake/scripts/flatbuffer_generate.cmake)
    target_link_libraries(${TARGET} PUBLIC FlatBuffers::FlatBuffers)
endif()

if (WK_SERIALIZATION_JSON)
    include(cmake/json.cmake)
    target_link_libraries(${TARGET} PUBLIC nlohmann_json::nlohmann_json)
endif()

# setup project
wk_project_setup(${TARGET})
