cmake_minimum_required(VERSION 3.26)
project(main C CXX)

add_executable(main)

target_sources(main
PRIVATE
    _main/main.c
)

target_include_directories(main PRIVATE
    .
    _main
    coms
)
