cmake_minimum_required(VERSION 3.12)

find_package(utf8cpp REQUIRED)

add_library(utf8help STATIC
    utf8help/utf8help.cpp
)

target_link_libraries(utf8help
PRIVATE
    utf8cpp
)
