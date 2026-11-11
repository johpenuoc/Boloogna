cmake_minimum_required(VERSION 3.26)
project(main C CXX)

add_executable(main)

target_sources(main
PRIVATE
    src/main.c
    coms/common.c
    src/syntax/scan.c
)

target_include_directories(main PRIVATE
    .
    src
    coms
    src/syntax
)
