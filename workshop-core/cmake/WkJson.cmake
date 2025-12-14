include(FetchContent)

function(wk_include_json)
    FetchContent_Declare(
        json
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz 
        FIND_PACKAGE_ARGS NAMES nlohmann-json nlohmann_json
    )
    FetchContent_MakeAvailable(json)
    target_link_libraries(${wk_core_target} PUBLIC nlohmann_json::nlohmann_json)
endfunction()