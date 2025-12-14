include(FetchContent)
include(scripts/flatbuffer_generate)

function(wk_include_flatbuffers)
    set(FLATBUFFERS_BUILD_FLATC ON)
    set(FLATBUFFERS_BUILD_SHAREDLIB ${WK_BUILD_SHARED})
    set(FLATBUFFERS_BUILD_TESTS OFF)
    FetchContent_Declare(
        flatbuffers
        GIT_REPOSITORY https://github.com/google/flatbuffers.git
        GIT_TAG v25.2.10
        FIND_PACKAGE_ARGS
    )
    FetchContent_MakeAvailable(flatbuffers)

    if (TARGET FlatBuffers::FlatBuffers)
        target_link_libraries(${wk_core_target} PUBLIC FlatBuffers::FlatBuffers)
    else()
        target_link_libraries(${wk_core_target} PUBLIC flatbuffers::flatbuffers)
    endif()
    
endfunction()