cmake_minimum_required (VERSION 3.12)

find_package(utf8cpp REQUIRED)
find_package(unofficial-nana REQUIRED)
find_package(fmt REQUIRED)

add_executable (RivalsReplayManager
    src/main.cpp
    src/ReplayRecord.cpp
)

target_include_directories(RivalsReplayManager
PRIVATE
${CMAKE_SOURCE_DIR}/utf8help
)

target_link_libraries(RivalsReplayManager
PRIVATE
    utf8cpp
    utf8help
    unofficial::nana::nana
    fmt::fmt
)
