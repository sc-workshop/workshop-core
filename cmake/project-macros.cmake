
# macro for setting up global variables
macro(wk_set_global var_name var_value)
    set(${var_name} ${var_value} CACHE INTERNAL "")
    add_compile_definitions(
        ${var_name}=${var_value}
    )
endmacro()

# function to setup specified project for workshop workspace
function(wk_project_setup project_name)
    # compilers
    wk_set_global(WK_GNU_DEF_fe "$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},GNU>")
    wk_set_global(WK_GNU_APPLE_fe "$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},AppleClang>")
    wk_set_global(WK_GNU_fe "$<OR:${WK_GNU_DEF_fe},${WK_GNU_APPLE_fe}>")
    wk_set_global(WK_MSVC_fe "$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>")

    wk_set_global(WK_CLANG "$<OR:${WK_GNU_fe},$<CXX_COMPILER_ID:Clang>>")
    wk_set_global(WK_GNU "$<OR:${WK_GNU_fe},$<CXX_COMPILER_ID:GNU>>")
    wk_set_global(WK_MSVC "$<OR:${WK_MSVC_fe},$<CXX_COMPILER_ID:MSVC>>")

    # build configurations
    wk_set_global(WK_DEBUG "$<CONFIG:Debug>")
    wk_set_global(WK_RELEASE "$<CONFIG:Release,RelWithDebInfo,MinSizeRel>")

    # build architectures
    wk_set_global(WK_X86_64 "$<OR:$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},AMD64>>")
    wk_set_global(WK_AARCH64 "$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},aarch64>")

    wk_set_global(WK_X64 "$<OR:${WK_X86_64},${WK_AARCH64}>")    

    wk_set_global(WK_PREFERRED_ISA "${WK_PREFERRED_CPU_FEATURES}")
    wk_set_global(WK_PREFERRED_LATEST_ISA "$<STREQUAL:${WK_PREFERRED_CPU_FEATURES},AVX2>")
    wk_set_global(WK_PREFERRED_OLDEST_ISA "$<STREQUAL:${WK_PREFERRED_CPU_FEATURES},DEFAULT>")

    # compile flags
    target_compile_options(${project_name} PRIVATE
        $<$<AND:${WK_MSVC},${WK_RELEASE}>: /O2 /GF /Gy /GS- /Ob2 /Oi /Ot> # Settings for release builds
        $<$<AND:${WK_MSVC},${WK_DEBUG}>: /W4> # Settings for debug builds

        $<${WK_MSVC}: /wd4820 /wd4365 /wd4061 /wd4514 /wd5219 /wd4242 /wd4711 /wd4710 /wd4625 /wd4626 /wd5039 /wd5045 /wd5026 /wd5027 /wd4623 /wd4201 /wd4099 /wd5267> # Disable stupid warnings

        $<$<AND:$<OR:${WK_GNU},${WK_CLANG}>,${WK_DEBUG}>: -Wextra -Wpedantic -Werror>
        $<$<OR:${WK_GNU},${WK_CLANG}>: -Wno-unused-variable -Wno-unknown-pragmas -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-c++98-compat -Wno-c++14-compat -Wno-error=microsoft-enum-value -Wno-error=language-extension-token> # Settings for GNU and Clang compilers

        $<$<AND:$<OR:${WK_GNU},${WK_CLANG}>,$<STREQUAL:${WK_PREFERRED_CPU_FEATURES},AVX2>>:-mavx2 -mbmi2 -maes -mpclmul -mfma> # AVX2
    )

    # compile defines
    target_compile_definitions(
            ${project_name} PRIVATE
            $<${WK_MSVC}: _CRT_SECURE_NO_WARNINGS _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING>
    )


    # DLL handling
    get_target_property(target_type ${project_name} TYPE)
    if (target_type STREQUAL "EXECUTABLE")
        target_compile_definitions(
            ${project_name} PRIVATE
            $<$<BOOL:${WK_BUILD_SHARED}>: WK_DLL_IMPORT=1>
        )
    else()
        target_compile_definitions(
            ${project_name} PRIVATE
            $<$<BOOL:${WK_BUILD_SHARED}>: WK_DLL_EXPORT=1>
        )
    endif ()

    # TODO(pavidloq): disable in-source builds and handle file grouping automatically.
    # File grouping
    if (NOT WK_IN_SOURCE_BUILD)
        get_target_property(SOURCES ${project_name} SOURCES)
        get_target_property(SOURCE_DIR ${project_name} SOURCE_DIR)
        source_group(TREE ${SOURCE_DIR}/source FILES ${SOURCES})
    endif()

    # force C++20
    target_compile_features(${project_name}
        PRIVATE
        cxx_std_20
    )

endfunction()
