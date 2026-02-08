include(FetchContent)

function(wk_include_blake3)
    FetchContent_Declare(
        blake3
        GIT_REPOSITORY https://github.com/BLAKE3-team/BLAKE3.git
        GIT_TAG 1.8.3
        SOURCE_SUBDIR c/
        FIND_PACKAGE_ARGS
    )
    FetchContent_MakeAvailable(blake3)

    target_link_libraries(${wk_core_target} PUBLIC BLAKE3::blake3)
    target_compile_definitions(${wk_core_target} PUBLIC WK_BLAKE3)
endfunction()