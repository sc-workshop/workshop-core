include(FetchContent)

function(wk_include_json)
    FetchContent_Declare(
        json
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz 
    )
    FetchContent_MakeAvailable(json)
    target_link_libraries(wk::core PUBLIC nlohmann_json::nlohmann_json)
endfunction()