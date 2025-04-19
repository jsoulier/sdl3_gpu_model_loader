cmake_minimum_required(VERSION 3.24)
project(example)

set(BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR})

add_subdirectory(SDL)
add_executable(example WIN32 example.c)
target_link_libraries(example PRIVATE SDL3::SDL3)
target_link_libraries(example PRIVATE SDL3_modelloader::SDL3_modelloader)
add_custom_command(
    OUTPUT ${BINARY_DIR}/example.frag
    COMMAND glslc example.frag -o ${BINARY_DIR}/example.frag
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS example.frag
    BYPRODUCTS ${BINARY_DIR}/example.frag
)
add_custom_command(
    OUTPUT ${BINARY_DIR}/example.vert
    COMMAND glslc example.vert -o ${BINARY_DIR}/example.vert
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS example.vert
    BYPRODUCTS ${BINARY_DIR}/example.vert
)
add_custom_target(example_frag DEPENDS ${BINARY_DIR}/example.frag)
add_custom_target(example_vert DEPENDS ${BINARY_DIR}/example.vert)
add_dependencies(example example_frag)
add_dependencies(example example_vert)