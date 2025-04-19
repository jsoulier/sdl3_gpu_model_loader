cmake_minimum_required(VERSION 3.24)
project(sample)

set(BINARY_DIR ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR})

add_subdirectory(SDL)
add_executable(sample WIN32 sample.c)
target_link_libraries(sample PRIVATE SDL3::SDL3)
target_link_libraries(sample PRIVATE SDL3_modelloader::SDL3_modelloader)
add_custom_command(
    OUTPUT ${BINARY_DIR}/sample.frag
    COMMAND glslc sample.frag -o ${BINARY_DIR}/sample.frag
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS sample.frag
    BYPRODUCTS ${BINARY_DIR}/sample.frag
)
add_custom_command(
    OUTPUT ${BINARY_DIR}/sample.vert
    COMMAND glslc sample.vert -o ${BINARY_DIR}/sample.vert
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS sample.vert
    BYPRODUCTS ${BINARY_DIR}/sample.vert
)
add_custom_target(sample_frag DEPENDS ${BINARY_DIR}/sample.frag)
add_custom_target(sample_vert DEPENDS ${BINARY_DIR}/sample.vert)
add_dependencies(sample sample_frag)
add_dependencies(sample sample_vert)