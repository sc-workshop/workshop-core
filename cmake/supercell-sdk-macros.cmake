
# macro for setting up global variables
macro(sc_set_global var_name var_value)
    set(${var_name} ${var_value} CACHE INTERNAL "")
    add_compile_definitions(
        ${var_name}=${var_value}
    )
endmacro()

# function to setup specified project for supercell workspace
function(sc_project_setup project_name)
    # compilers
    sc_set_global(SC_GNU_DEF_fe "$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},GNU>")
    sc_set_global(SC_GNU_APPLE_fe "$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},AppleClang>")
    sc_set_global(SC_GNU_fe "$<OR:${SC_GNU_DEF_fe},${SC_GNU_APPLE_fe}>")
    sc_set_global(SC_MSVC_fe "$<STREQUAL:${CMAKE_CXX_COMPILER_FRONTEND_VARIANT},MSVC>")

    sc_set_global(SC_CLANG "$<OR:${SC_GNU_fe},$<CXX_COMPILER_ID:Clang>>")
    sc_set_global(SC_GNU "$<OR:${SC_GNU_fe},$<CXX_COMPILER_ID:GNU>>")
    sc_set_global(SC_MSVC "$<OR:${SC_MSVC_fe},$<CXX_COMPILER_ID:MSVC>>")

    # build configurations
    sc_set_global(SC_DEBUG "$<CONFIG:Debug>")
    sc_set_global(SC_RELEASE "$<CONFIG:Release,RelWithDebInfo,MinSizeRel>")

    # build architectures
    sc_set_global(SC_X86_64 "$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>")
    sc_set_global(SC_AARCH64 "$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},aarch64>")

    sc_set_global(SC_X64 "$<OR:${SC_X86_64},${SC_AARCH64}>")

    # compile flags
    target_compile_options(${project_name} PRIVATE
        $<$<AND:${SC_MSVC},${SC_RELEASE}>: /Wall /Ox /GF /Gy /GS- /Ob2 /Oi /Ot> # Settings for release builds

        $<${SC_MSVC}: /wd4820 /wd4365 /wd4061 /wd4514 /wd5219 /wd4242 /wd4711 /wd4710 /wd4625 /wd4626 /wd5039 /wd5045 /wd5026 /wd5027 /wd4623 /wd4201> # Disable stupid warnings

        $<$<OR:${SC_GNU},${SC_CLANG}>:-Wall -Wextra -Wpedantic -Wno-unused-variable -Wno-unknown-pragmas -Werror -Wno-gnu-anonymous-struct -Wno-nested-anon-types> # Settings for GNU and Clang compilers
    )

    # compile defines
    target_compile_definitions(
            ${project_name} PRIVATE
            $<${SC_MSVC}: _CRT_SECURE_NO_WARNINGS>
    )


    # Dll handling
    get_target_property(target_type ${project_name} TYPE)
    if (target_type STREQUAL "EXECUTABLE")
        target_compile_definitions(
            ${project_name} PRIVATE
            $<$<BOOL:${SC_BUILD_SHARED}>: SC_DLL_IMPORT=1>
        )
    else()
        target_compile_definitions(
            ${project_name} PRIVATE
            $<$<BOOL:${SC_BUILD_SHARED}>: SC_DLL_EXPORT=1>
        )
    endif ()

    # File grouping
    if (NOT SC_IN_SOURCE_BUILD)
        get_target_property(SOURCES ${project_name} SOURCES)
        get_target_property(SOURCE_DIR ${project_name} SOURCE_DIR)
        source_group(TREE ${SOURCE_DIR}/source FILES ${SOURCES})
    endif()

    # force C++17
    target_compile_features(${project_name}
        PRIVATE
        cxx_std_17
    )

endfunction()
