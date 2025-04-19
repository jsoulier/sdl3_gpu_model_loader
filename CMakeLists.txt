cmake_minimum_required(VERSION 3.24)
project(SDL3_modelloader)

add_library(SDL3_modelloader
    external/cgltf/cgltf.c
    external/fast_obj/fast_obj.c
    external/stb/stb.c
    src/SDL_modelloader.c
    src/SDL_modelloader_gltf.c
    src/SDL_modelloader_obj.c
)
add_library(SDL3_modelloader::SDL3_modelloader ALIAS SDL3_modelloader)
if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(example)
endif()
target_link_libraries(SDL3_modelloader PRIVATE SDL3::SDL3)
target_include_directories(SDL3_modelloader PRIVATE external/cgltf)
target_include_directories(SDL3_modelloader PRIVATE external/fast_obj)
target_include_directories(SDL3_modelloader PRIVATE external/stb)
target_include_directories(SDL3_modelloader PUBLIC include)