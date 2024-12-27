
function(wk_include_xxhash)
    add_subdirectory(../source/core/3rdparty/xxhash)
    target_link_libraries(${wk_core_target} PUBLIC xxhash::xxHash)
endfunction()