FILE(GLOB_RECURSE grid_headers "./grid/*.hpp")
FILE(GLOB_RECURSE utils_headers "./utils/*.hpp")

add_library(src INTERFACE)

target_include_directories(src INTERFACE grid utils)

install(TARGETS Searching_Canvas EXPORT src)