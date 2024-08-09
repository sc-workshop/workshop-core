include(FetchContent)

set(FLATBUFFERS_BUILD_FLATC OFF)
set(FLATBUFFERS_BUILD_SHAREDLIB ${SC_BUILD_SHARED})
set(FLATBUFFERS_BUILD_TESTS OFF)
FetchContent_Declare(
    flatbuffers
    GIT_REPOSITORY https://github.com/google/flatbuffers.git
    GIT_TAG v24.3.25
)
FetchContent_MakeAvailable(flatbuffers)