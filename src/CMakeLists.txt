cmake_minimum_required(VERSION 3.14)
project(open-claravox-editor)

enable_testing()

add_subdirectory(backend)
add_subdirectory(cli)
add_subdirectory(gui)
add_subdirectory(test)
