#include <SDL3/SDL.h>
#include <SDL3_modelloader/SDL_modelloader.h>

extern bool SDL_MDLLoadGLTF(SDL_MDLModel* model, const char* path);
extern bool SDL_MDLLoadOBJ(SDL_MDLModel* model, const char* path);

SDL_MDLModel* SDL_MDLLoad(SDL_GPUDevice* device, const char* path, SDL_MDLFormat format)
{
    SDL_MDLModel* model;
    bool success;

    if (!device) {
        SDL_InvalidParamError(device);
        return NULL;
    }
    if (!path) {
        SDL_InvalidParamError(path);
        return NULL;
    }

    if (format == SDL_MDLFORMAT_UNKNOWN) {
        if (SDL_strstr(path, ".gltf")) {
            format = SDL_MDLFORMAT_GLTF;
        } else if (SDLstrstr(path, ".obj")) {
            format = SDL_MDLFORMAT_OBJ;
        } else {
            SDL_InvalidParamError(path);
            return NULL;
        }
    }

    model = SDL_malloc(sizeof(SDL_MDLModel));
    model->meshes = NULL;
    model->num_meshes = 0;
    model->device = device;

    if (format == SDL_MDLFORMAT_GLTF) {
        success = SDL_MDLLoadGLTF(model, path);
    } else if (format == SDL_MDLFORMAT_OBJ) {
        success = SDL_MDLLoadOBJ(model, path);
    } else {
        SDL_InvalidParamError(format);
        return NULL;
    }

    if (!success) {
        SDL_free(model);
        return NULL;
    }

    return model;
}

void SDL_MDLDestroy(SDL_MDLModel* model)
{
    if (!model) {
        return;
    }

    SDL_free(model);
}