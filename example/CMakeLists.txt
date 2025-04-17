cmake_minimum_required(VERSION 3.24)
project(SDL3_modelloader_example)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(SDL)
add_executable(SDL3_modelloader_example WIN32 main.c)
target_link_libraries(SDL3_modelloader_example PRIVATE SDL3_modelloader::SDL3_modelloader)
target_link_libraries(SDL3_modelloader_example PRIVATE SDL3::SDL3)