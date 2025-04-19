#ifndef SDL_MODELLOADER_IMPL_H_
#define SDL_MODELLOADER_IMPL_H_

#include <SDL3/SDL.h>
#include <SDL3_modelloader/SDL_modelloader.h>

bool SDL_ModelLoadGLTF(SDL_Model* model, const char* path);
bool SDL_ModelLoadOBJ(SDL_Model* model, const char* path);

#endif