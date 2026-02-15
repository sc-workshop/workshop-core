# Copyright 2015 Google Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Creates a target that can be linked against that generates flatbuffer headers.
#
# This function takes a target name and a list of schemas. You can also specify
# other flagc flags using the FLAGS option to change the behavior of the flatc
# tool.
#
# When the target_link_libraries is done within a different directory than
# flatbuffers_generate_headers is called, then the target should also be dependent
# the custom generation target called GENERATE_<TARGET>.
#
# Arguments:
#   TARGET: The name of the target to generate.
#   SCHEMAS: The list of schema files to generate code for.
#   BINARY_SCHEMAS_DIR: Optional. The directory in which to generate binary
#       schemas. Binary schemas will only be generated if a path is provided.
#   INCLUDE: Optional. Search for includes in the specified paths. (Use this
#       instead of "-I <path>" and the FLAGS option so that CMake is aware of
#       the directories that need to be searched).
#   INCLUDE_PREFIX: Optional. The directory in which to place the generated
#       files. Use this instead of the --include-prefix option.
#   FLAGS: Optional. A list of any additional flags that you would like to pass
#       to flatc.
#
# Example:
#
#     flatbuffers_generate_headers(
#         TARGET my_generated_headers_target
#         INCLUDE_PREFIX ${MY_INCLUDE_PREFIX}"
#         SCHEMAS ${MY_SCHEMA_FILES}
#         BINARY_SCHEMAS_DIR "${MY_BINARY_SCHEMA_DIRECTORY}"
#         FLAGS --gen-object-api)
#
#     target_link_libraries(MyExecutableTarget
#         PRIVATE my_generated_headers_target
#     )
#
# Optional (only needed within different directory):
#     add_dependencies(app GENERATE_my_generated_headers_target)
function(wk_flatbuffers_generate_headers)
  if (MSVC)
    wk_flatbuffers_generate_headers_msvc(${ARGV})
  else()
    wk_flatbuffers_generate_headers_gnu(${ARGV})
  endif()

endfunction()

function(wk_flatbuffers_generate_headers_msvc)
  # Parse function arguments.
  set(options)
  set(one_value_args
    "TARGET"
    "INCLUDE_PREFIX"
    "BINARY_SCHEMAS_DIR")
  set(multi_value_args
    "SCHEMAS"
    "INCLUDE"
    "FLAGS")
  cmake_parse_arguments(
    PARSE_ARGV 0
    FLATBUFFERS_GENERATE_HEADERS
    "${options}"
    "${one_value_args}"
    "${multi_value_args}")

  # Test if including from FindFlatBuffers
  if(FLATBUFFERS_FLATC_EXECUTABLE)
    set(FLATC_TARGET "")
    set(FLATC ${FLATBUFFERS_FLATC_EXECUTABLE})
  elseif(TARGET flatbuffers::flatc)
    set(FLATC_TARGET flatbuffers::flatc)
    set(FLATC flatbuffers::flatc)
  else()
    set(FLATC_TARGET flatc)
    set(FLATC flatc)

    find_program(FLATC_EXE flatc)
    if(FLATC_EXE STREQUAL "")
      message(FATAL_ERROR "flatc not found")
    endif()
  endif()

  set(working_dir "${CMAKE_CURRENT_SOURCE_DIR}")

  # Generate the include files parameters.
  set(include_params "")
  foreach (include_dir ${FLATBUFFERS_GENERATE_HEADERS_INCLUDE})
    set(include_params -I ${include_dir} ${include_params})
  endforeach()

  # Create a directory to place the generated code.
  set(generated_target_dir "${CMAKE_CURRENT_BINARY_DIR}/${FLATBUFFERS_GENERATE_HEADERS_TARGET}")
  set(generated_include_dir "${generated_target_dir}")
  if (NOT ${FLATBUFFERS_GENERATE_HEADERS_INCLUDE_PREFIX} STREQUAL "")
    set(generated_include_dir "${generated_include_dir}/${FLATBUFFERS_GENERATE_HEADERS_INCLUDE_PREFIX}")
    list(APPEND FLATBUFFERS_GENERATE_HEADERS_FLAGS 
         "--include-prefix" ${FLATBUFFERS_GENERATE_HEADERS_INCLUDE_PREFIX})
  endif()

  set(generated_custom_commands)

  # Create rules to generate the code for each schema.
  foreach(schema ${FLATBUFFERS_GENERATE_HEADERS_SCHEMAS})
    get_filename_component(filename ${schema} NAME_WE)
    set(generated_include "${generated_include_dir}/${filename}_generated.h")

    # Generate files for grpc if needed
    set(generated_source_file)
    if("${FLATBUFFERS_GENERATE_HEADERS_FLAGS}" MATCHES "--grpc")
      # Check if schema file contain a rpc_service definition
      file(STRINGS ${schema} has_grpc REGEX "rpc_service")
      if(has_grpc)
        list(APPEND generated_include "${generated_include_dir}/${filename}.grpc.fb.h")
        set(generated_source_file "${generated_include_dir}/${filename}.grpc.fb.cc")
      endif()
    endif()

    message("Processing file: ${schema}")
    execute_process(
        COMMAND ${FLATC} ${FLATC_ARGS}
        -o ${generated_include_dir}
        ${include_params}
        -c ${schema}
        ${FLATBUFFERS_GENERATE_HEADERS_FLAGS}
        WORKING_DIRECTORY "${working_dir}"
    )
    list(APPEND all_generated_header_files ${generated_include})
    list(APPEND all_generated_source_files ${generated_source_file})
    list(APPEND generated_custom_commands "${generated_include}")

    # Geneate the binary flatbuffers schemas if instructed to.
    if (NOT ${FLATBUFFERS_GENERATE_HEADERS_BINARY_SCHEMAS_DIR} STREQUAL "")
      set(binary_schema
          "${FLATBUFFERS_GENERATE_HEADERS_BINARY_SCHEMAS_DIR}/${filename}.bfbs")
      add_custom_command(
        OUTPUT ${binary_schema}
        COMMAND ${FLATC} -b --schema
        -o ${FLATBUFFERS_GENERATE_HEADERS_BINARY_SCHEMAS_DIR}
        ${include_params}
        ${schema}
        DEPENDS ${FLATC_TARGET} ${schema}
        WORKING_DIRECTORY "${working_dir}")
      list(APPEND generated_custom_commands "${binary_schema}")
      list(APPEND all_generated_binary_files ${binary_schema})
    endif()
  endforeach()

  # Set up interface library
  add_library(${FLATBUFFERS_GENERATE_HEADERS_TARGET} INTERFACE)
  target_include_directories(
    ${FLATBUFFERS_GENERATE_HEADERS_TARGET}
    INTERFACE ${generated_target_dir})

  # Organize file layout for IDEs.
  source_group(
    TREE "${generated_target_dir}"
    PREFIX "Flatbuffers/Generated/Headers Files"
    FILES ${all_generated_header_files})
  source_group(
    TREE "${generated_target_dir}"
    PREFIX "Flatbuffers/Generated/Source Files"
    FILES ${all_generated_source_files})
  source_group(
    TREE ${working_dir}
    PREFIX "Flatbuffers/Schemas"
    FILES ${FLATBUFFERS_GENERATE_HEADERS_SCHEMAS})
  if (NOT ${FLATBUFFERS_GENERATE_HEADERS_BINARY_SCHEMAS_DIR} STREQUAL "")
    source_group(
      TREE "${FLATBUFFERS_GENERATE_HEADERS_BINARY_SCHEMAS_DIR}"
      PREFIX "Flatbuffers/Generated/Binary Schemas"
      FILES ${all_generated_binary_files})
  endif()
endfunction()

function(wk_flatbuffers_generate_headers_gnu)
  # Parse function arguments.
  set(options)
  set(one_value_args
    "TARGET"
    "INCLUDE_PREFIX"
    "BINARY_SCHEMAS_DIR")
  set(multi_value_args
    "SCHEMAS"
    "INCLUDE"
    "FLAGS")
  cmake_parse_arguments(
    PARSE_ARGV 0
    FLATBUFFERS_GENERATE_HEADERS
    "${options}"
    "${one_value_args}"
    "${multi_value_args}")

  # Test if including from FindFlatBuffers
  if(FLATBUFFERS_FLATC_EXECUTABLE)
    set(FLATC_TARGET "")
    set(FLATC ${FLATBUFFERS_FLATC_EXECUTABLE})
  elseif(TARGET flatbuffers::flatc)
    set(FLATC_TARGET flatbuffers::flatc)
    set(FLATC flatbuffers::flatc)
  else()
    set(FLATC_TARGET flatc)
    set(FLATC flatc)
  endif()

  set(working_dir "${CMAKE_CURRENT_SOURCE_DIR}")

  # Generate the include files parameters.
  set(include_params "")
  foreach (include_dir ${FLATBUFFERS_GENERATE_HEADERS_INCLUDE})
    set(include_params -I ${include_dir} ${include_params})
  endforeach()

  # Create a directory to place the generated code.
  set(generated_target_dir "${CMAKE_CURRENT_BINARY_DIR}/${FLATBUFFERS_GENERATE_HEADERS_TARGET}")
  set(generated_include_dir "${generated_target_dir}")
  if (NOT ${FLATBUFFERS_GENERATE_HEADERS_INCLUDE_PREFIX} STREQUAL "")
    set(generated_include_dir "${generated_include_dir}/${FLATBUFFERS_GENERATE_HEADERS_INCLUDE_PREFIX}")
    list(APPEND FLATBUFFERS_GENERATE_HEADERS_FLAGS 
         "--include-prefix" ${FLATBUFFERS_GENERATE_HEADERS_INCLUDE_PREFIX})
  endif()

  set(generated_custom_commands)

  # Create rules to generate the code for each schema.
  foreach(schema ${FLATBUFFERS_GENERATE_HEADERS_SCHEMAS})
    get_filename_component(filename ${schema} NAME_WE)
    set(generated_include "${generated_include_dir}/${filename}_generated.h")

    # Generate files for grpc if needed
    set(generated_source_file)
    if("${FLATBUFFERS_GENERATE_HEADERS_FLAGS}" MATCHES "--grpc")
      # Check if schema file contain a rpc_service definition
      file(STRINGS ${schema} has_grpc REGEX "rpc_service")
      if(has_grpc)
        list(APPEND generated_include "${generated_include_dir}/${filename}.grpc.fb.h")
        set(generated_source_file "${generated_include_dir}/${filename}.grpc.fb.cc")
      endif()
    endif()

    add_custom_command(
      OUTPUT ${generated_include} ${generated_source_file}
      COMMAND ${FLATC} ${FLATC_ARGS}
      -o ${generated_include_dir}
      ${include_params}
      -c ${schema}
      ${FLATBUFFERS_GENERATE_HEADERS_FLAGS}
      DEPENDS ${FLATC_TARGET} ${schema}
      WORKING_DIRECTORY "${working_dir}"
      COMMENT "Building ${schema} flatbuffers...")
    list(APPEND all_generated_header_files ${generated_include})
    list(APPEND all_generated_source_files ${generated_source_file})
    list(APPEND generated_custom_commands "${generated_include}" "${generated_source_file}")

    # Geneate the binary flatbuffers schemas if instructed to.
    if (NOT ${FLATBUFFERS_GENERATE_HEADERS_BINARY_SCHEMAS_DIR} STREQUAL "")
      set(binary_schema
          "${FLATBUFFERS_GENERATE_HEADERS_BINARY_SCHEMAS_DIR}/${filename}.bfbs")
      add_custom_command(
        OUTPUT ${binary_schema}
        COMMAND ${FLATC} -b --schema
        -o ${FLATBUFFERS_GENERATE_HEADERS_BINARY_SCHEMAS_DIR}
        ${include_params}
        ${schema}
        DEPENDS ${FLATC_TARGET} ${schema}
        WORKING_DIRECTORY "${working_dir}")
      list(APPEND generated_custom_commands "${binary_schema}")
      list(APPEND all_generated_binary_files ${binary_schema})
    endif()
  endforeach()

  # Create an additional target as add_custom_command scope is only within same directory (CMakeFile.txt)
  set(generate_target GENERATE_${FLATBUFFERS_GENERATE_HEADERS_TARGET})
  add_custom_target(${generate_target}
                  DEPENDS ${generated_custom_commands}
                  COMMENT "Generating flatbuffer target ${FLATBUFFERS_GENERATE_HEADERS_TARGET}")

  # Set up interface library
  add_library(${FLATBUFFERS_GENERATE_HEADERS_TARGET} INTERFACE)
  add_dependencies(
    ${FLATBUFFERS_GENERATE_HEADERS_TARGET}
    ${FLATC}
    ${FLATBUFFERS_GENERATE_HEADERS_SCHEMAS}
    ${generate_target}
  )
  target_include_directories(
    ${FLATBUFFERS_GENERATE_HEADERS_TARGET}
    INTERFACE ${generated_target_dir})

  # Organize file layout for IDEs.
  source_group(
    TREE ${working_dir}
    PREFIX "Flatbuffers/Schemas"
    FILES ${FLATBUFFERS_GENERATE_HEADERS_SCHEMAS})
  if (NOT ${FLATBUFFERS_GENERATE_HEADERS_BINARY_SCHEMAS_DIR} STREQUAL "")
    source_group(
      TREE "${FLATBUFFERS_GENERATE_HEADERS_BINARY_SCHEMAS_DIR}"
      PREFIX "Flatbuffers/Generated/Binary Schemas"
      FILES ${all_generated_binary_files})
  endif()
endfunction()