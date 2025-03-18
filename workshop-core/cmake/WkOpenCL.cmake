function(wk_include_opencl)
    set(OPENCL_HEADERS_BUILD_CXX_TESTS OFF)
    set(OPENCL_SDK_BUILD_UTILITY_LIBRARIES OFF)
    set(DEPENDENCIES_FORCE_DOWNLOAD ON CACHE INTERNAL "")
    FetchContent_Declare(
        OpenCL
        GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-SDK.git
        GIT_TAG v2024.10.24
    )
    FetchContent_MakeAvailable(OpenCL)
    target_link_libraries(
        ${wk_core_target} PUBLIC 
        OpenCL::OpenCL
        OpenCL::HeadersCpp
        #OpenCL::SDKCpp
    )
endfunction()

#
# Arguments:
#   NAME: Name of target
#   SOURCE: Paths to .cl source files
#   OUTPUT: Name of output folders
#
# Example:
#
function(wk_create_opencl_target)
    set(options)
    set(one_value_args "NAME" "OUTPUT")
    set(multi_value_args "SOURCE")
    cmake_parse_arguments(
        PARSE_ARGV 0
        CL
        "${options}"
        "${one_value_args}"
        "${multi_value_args}"
    )

    set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/WkOpenCL/${CL_NAME}")
    set(CL_INCLUDE "${GENERATED_DIR}")

    if (NOT "${CL_OUTPUT}" STREQUAL "")
        set(GENERATED_DIR "${GENERATED_DIR}/${CL_OUTPUT}")
    endif()

    set(sources "")
    set(headers "")
    foreach(source "${CL_SOURCE}")
        get_filename_component(filename "${source}" NAME_WE)
        list(APPEND sources "${CMAKE_CURRENT_SOURCE_DIR}/source/${source}")
        list(APPEND headers "${GENERATED_DIR}/${filename}_gen.h")
    endforeach()

    set(GENERATOR_TARGET "${CL_NAME}_cl_gen")
    add_custom_target(${GENERATOR_TARGET} 
        DEPENDS ${sources}
        SOURCES ${sources}
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/scripts/opencl_generate.cmake ${sources} ${headers}
    )
    set_source_files_properties(${GENERATOR_TARGET} PROPERTIES SYMBOLIC 1)

    add_library(${CL_NAME} INTERFACE)
    target_include_directories(${CL_NAME} INTERFACE "${CL_INCLUDE}")

endfunction()