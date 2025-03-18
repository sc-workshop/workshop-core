include("${CMAKE_CURRENT_LIST_DIR}/../WkBin2h.cmake")

SET(INPUT ${CMAKE_ARGV3})
SET(OUTPUT ${CMAKE_ARGV4})

foreach(FILE IN ZIP_LISTS INPUT OUTPUT)
    get_filename_component(filename "${FILE_0}" NAME_WE)
    set(SOURCE "${filename}ProgramSource")
    set(OTHER_DECLARATIONS 
        "static wk::parallel::clProgramBuilder ${filename}Program = ${SOURCE}"
    )
    

    wk_bin2h(
        SOURCE_FILE "${FILE_0}"
        VARIABLE_NAME "${SOURCE}"
        HEADER_FILE "${FILE_1}"
        OTHER_DATA ${OTHER_DECLARATIONS}
    )
endforeach()
