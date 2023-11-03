cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Searching_Canvas)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID: CXX, ARMClang, AppleClang, Clang, GNU, LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID: CXX, MSVC>")

add_executable(Searching_Canvas main.cpp)

target_include_directories(Searching_Canvas PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
)

target_compile_options(Searching_Canvas INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE: -Wall; -Wextra; -Wunused; -fconcepts; -fconcepts-ts; -fsplit-stack; -O3>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE: -W3>>"
)

add_subdirectory(src)

target_link_libraries(Searching_Canvas PUBLIC src)