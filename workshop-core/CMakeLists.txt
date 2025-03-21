# Project: workshop-core

# C++ headers and source files
file(GLOB_RECURSE HEADERS source/*.h source/*.hpp)
file(GLOB_RECURSE SOURCES source/*.cpp)

# Remove OpenCL source by default
if (NOT ${WK_USE_OPENCL})
    file(GLOB_RECURSE CL_SOURCE source/core/parallel/cl/*.cpp)
    list(REMOVE_ITEM SOURCES ${CL_SOURCE})
endif()

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

# Target
set(wk_core_target "${TARGET}" CACHE INTERNAL "")
add_library(wk::core ALIAS ${TARGET})
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "WorkshopCore")

set_target_properties(${TARGET} PROPERTIES
    FOLDER WorkshopSDK/core
)

# add *.h and *.cpp files
target_sources(${TARGET} PRIVATE ${HEADERS} ${SOURCES})

# include directories
target_include_directories(${TARGET} PUBLIC source)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE INTERNAL "")

# setup project
wk_project_setup(${TARGET})
