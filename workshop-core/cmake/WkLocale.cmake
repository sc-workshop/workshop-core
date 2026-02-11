include(FetchContent)

function(wk_include_locale)
    set(SIMDUTF_TESTS OFF)
    set(SIMDUTF_TOOLS OFF)

    FetchContent_Declare(
        simdutf
        GIT_REPOSITORY https://github.com/simdutf/simdutf.git
        GIT_TAG v8.0.0
    )
    FetchContent_MakeAvailable(simdutf)

    target_link_libraries(${wk_core_target} PUBLIC simdutf::simdutf)
    target_compile_definitions(${wk_core_target} PUBLIC WK_LOCALE)
endfunction()