
# macro for setting up global variables
macro(wk_set_global var_name var_value)
    set(${var_name} ${var_value} CACHE INTERNAL "")
    add_compile_definitions(
        ${var_name}=${var_value}
    )
endmacro()

# function to setup specified project for workshop workspace
macro(wk_project_setup project_name)
    # Compiler accepts AppleClang-style command line options, which is also GNU-style
    wk_set_global(WK_CLANG "$<CXX_COMPILER_ID:Clang,AppleClang>")

    # Compiler accepts GNU-style command line options
    wk_set_global(WK_GNU "$<CXX_COMPILER_ID:GNU>")

    # Compiler accepts MSVC-style command line options
    wk_set_global(WK_MSVC "$<CXX_COMPILER_ID:MSVC>")

    # Xcode generator
    wk_set_global(WK_XCODE "$<STREQUAL:${CMAKE_GENERATOR},Xcode>")

    # Compiler is Visual Studio cl.exe
    wk_set_global(WK_MSVC_CL "$<AND:${WK_MSVC},$<CXX_COMPILER_ID:MSVC>>")

    # Compiler is Visual Studio clangcl.exe
    set(WK_CLANG_CL "$<AND:${WK_MSVC},$<CXX_COMPILER_ID:Clang>>")

    # Build configurations
    wk_set_global(WK_DEBUG "$<CONFIG:Debug>")
    wk_set_global(WK_RELEASE "$<CONFIG:Release,RelWithDebInfo,MinSizeRel>")

    # Build architectures
    wk_set_global(WK_X86_64 "$<OR:$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},AMD64>>")
    wk_set_global(WK_AARCH64 "$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},aarch64>")

    wk_set_global(WK_X64 "$<OR:${WK_X86_64},${WK_AARCH64}>")

    # Simd
    wk_set_global(WK_AVX2_GNU "$<AND:$<OR:${WK_GNU},${WK_CLANG}>,$<STREQUAL:${WK_PREFERRED_CPU_FEATURES},AVX2>,$<NOT:${WK_XCODE}>,${WK_X86_64}>")
    wk_set_global(WK_AVX2_MSVC "$<AND:${WK_MSVC},$<STREQUAL:${WK_PREFERRED_CPU_FEATURES},AVX2>>")

    # Note: Moved down because of MacOS multi arch
    #wk_set_global(WK_AVX2 "$<OR:${WK_AVX2_GNU},${WK_AVX2_MSVC}>")

    wk_set_global(WK_PREFERRED_ISA "${WK_PREFERRED_CPU_FEATURES}")
    wk_set_global(WK_PREFERRED_LATEST_ISA "$<STREQUAL:${WK_PREFERRED_CPU_FEATURES},AVX2>")
    wk_set_global(WK_PREFERRED_OLDEST_ISA "$<STREQUAL:${WK_PREFERRED_CPU_FEATURES},DEFAULT>")

    # Compile flags
    target_compile_options(${project_name} PRIVATE
        $<$<AND:${WK_MSVC},${WK_RELEASE}>: /O2 /GF /Gy /GS- /Ob2 /Oi /Ot> # Settings for release builds
        $<$<AND:${WK_MSVC},${WK_DEBUG}>: /W4> # Settings for debug builds

        $<${WK_MSVC}: /wd4820 /wd4365 /wd4061 /wd4514 /wd5219 /wd4242 /wd4711 /wd4710 /wd4625 /wd4626 /wd5039 /wd5045 /wd5026 /wd5027 /wd4623 /wd4201 /wd4099 /wd5267> # Disable stupid warnings

        $<$<AND:$<OR:${WK_GNU},${WK_CLANG}>,${WK_DEBUG}>: -Wextra -Wpedantic -Werror>
        $<$<OR:${WK_GNU},${WK_CLANG}>: -Wno-unused-variable -Wno-unknown-pragmas -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-c++98-compat -Wno-c++14-compat> # Shared settings between GNU and Clang compilers
        $<${WK_CLANG}: -Wno-error=microsoft-enum-value -Wno-error=language-extension-token>

        # AVX2 on Windows
        $<${WK_AVX2_MSVC}: /arch:AVX2 >
        $<${WK_AVX2_GNU}: -mavx2 -mbmi2 -maes -mpclmul -mfma>
    )

    if(WK_XCODE)
        # AVX2 only on haswell
        if(CMAKE_OSX_ARCHITECTURES MATCHES x86_64h)
            # I want to believe that there is more... pretty way to do this
            # But this is the only one that really worked and really passed flags to Clang
            set_target_properties(${project_name} PROPERTIES
                XCODE_ATTRIBUTE_OTHER_CPLUSPLUSFLAGS[arch=x86_64h] "$(inherited) -mavx2 -mpopcnt -mf16c -DWK_AVX2=1"
            )
        endif()

        # Disable WK_AVX2 define on other archs
        foreach(arch IN LISTS CMAKE_OSX_ARCHITECTURES)
            if(NOT arch STREQUAL "x86_64h")
                set_property(TARGET ${target} APPEND PROPERTY
                    XCODE_ATTRIBUTE_OTHER_CPLUSPLUSFLAGS[arch=${arch}]
                    "$(inherited) -DWK_AVX2=0"
                )
            endif()
        endforeach()
    else()
        # Define WK_AVX2 as usual on other platforms
        wk_set_global(WK_AVX2 "$<OR:${WK_AVX2_GNU},${WK_AVX2_MSVC}>")
    endif()

    # Remove annoying warnings for MSVC
    target_compile_definitions(
        ${project_name} PRIVATE
        $<${WK_MSVC}: _CRT_SECURE_NO_WARNINGS _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING>
    )

    # DLL handling
    get_target_property(target_type ${project_name} TYPE)
    if(target_type STREQUAL "EXECUTABLE")
        target_compile_definitions(
            ${project_name} PRIVATE
            $<$<BOOL:${WK_BUILD_SHARED}>: WK_DLL_IMPORT=1>
        )
    else()
        target_compile_definitions(
            ${project_name} PRIVATE
            $<$<BOOL:${WK_BUILD_SHARED}>: WK_DLL_EXPORT=1>
        )
    endif()

    # TODO(pavidloq): disable in-source builds and handle file grouping automatically.
    # File grouping
    if(NOT WK_IN_SOURCE_BUILD)
        get_target_property(SOURCES ${project_name} SOURCES)
        get_target_property(SOURCE_DIR ${project_name} SOURCE_DIR)
        source_group(TREE ${SOURCE_DIR}/source FILES ${SOURCES})
    endif()

    # force C++20
    target_compile_features(${project_name}
        PRIVATE
        cxx_std_20
    )

endmacro()

macro(wk_fast_math project_name)
    target_compile_options(${project_name} PRIVATE
        $<$<AND:${WK_MSVC_CL},$<VERSION_LESS:$<CXX_COMPILER_VERSION>,19.30>>: /fp:fast >
        $<$<AND:${WK_MSVC_CL},$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,19.30>>: /fp:fast >
        $<${WK_CLANG_CL}: /fp:fast >

        $<$<AND:${WK_CLANG_CL},$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,14.0.0>>: -Xclang$<SEMICOLON>-ffp-contract=fast >
        $<$<AND:${WK_CLANG},$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,10.0.0>>: -ffp-model=fast >

        $<${WK_GNU}: -ffp-contract=fast >
        $<${WK_GNU}: -fno-math-errno >
        $<${WK_GNU}: -fno-trapping-math >

        # Hide noise from clang and clangcl 20 warning the 2nd fp option changes
        # one of the settings made the first.
        $<$<AND:${WK_CLANG},$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,20.0.0>>: -Wno-overriding-option >
    )
endmacro()

macro(wk_strict_math project_name)
    target_compile_options(${project_name} PRIVATE
        $<$<AND:${WK_MSVC_CL},$<VERSION_LESS:$<CXX_COMPILER_VERSION>,19.30>>: /fp:strict >
        $<$<AND:${WK_MSVC_CL},$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,19.30>>: /fp:precise >
        $<${WK_CLANG_CL}: /fp:precise >

        $<$<AND:${WK_CLANG_CL},$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,14.0.0>>: -Xclang$<SEMICOLON>-ffp-contract=off >
        $<$<AND:${WK_CLANG},$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,10.0.0>>: -ffp-model=precise >
        $<${WK_GNU}: -ffp-contract=off >

        $<$<AND:${WK_CLANG},$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,20.0.0>>: -Wno-overriding-option >
    )
endmacro()
