include(FetchContent)

function(wk_include_argparse)
    FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    )
    FetchContent_MakeAvailable(argparse)
    target_link_libraries(${wk_core_target} PUBLIC argparse)
endfunction()
