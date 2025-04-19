#include "SDL_modelloader_impl.h"

SDL_Model* SDL_ModelLoad(SDL_GPUDevice* device, const char* path, SDL_ModelFormat format)
{
    SDL_Model* model;
    bool success;

    if (!device) {
        SDL_InvalidParamError(device);
        return NULL;
    }
    if (!path) {
        SDL_InvalidParamError(path);
        return NULL;
    }

    if (format == SDL_ModelFORMAT_UNKNOWN) {
        if (SDL_strstr(path, ".gltf")) {
            format = SDL_ModelFORMAT_GLTF;
        } else if (SDLstrstr(path, ".obj")) {
            format = SDL_ModelFORMAT_OBJ;
        } else {
            SDL_InvalidParamError(path);
            return NULL;
        }
    }

    model = SDL_malloc(sizeof(SDL_Model));
    model->meshes = NULL;
    model->num_meshes = 0;
    model->materials = NULL;
    model->num_materials = 0;
    model->device = device;

    if (format == SDL_ModelFORMAT_GLTF) {
        success = SDL_ModelLoadGLTF(model, path);
    } else if (format == SDL_ModelFORMAT_OBJ) {
        success = SDL_ModelLoadOBJ(model, path);
    } else {
        SDL_InvalidParamError(format);
        success = false;
    }

    if (!success) {
        SDL_free(model);
        return NULL;
    }

    return model;
}

void SDL_ModelDestroy(SDL_Model* model)
{
    if (!model) {
        return;
    }

    for (Uint32 i = 0; i < model->num_meshes; i++) {
        SDL_ModelMesh* mesh = &model->meshes[i];
        if (mesh->vertices) {
            SDL_ReleaseGPUBuffer(model->device, mesh->vertices);
        }
        if (mesh->indices) {
            SDL_ReleaseGPUBuffer(model->device, mesh->indices);
        }
    }

    for (Uint32 i = 0; i < model->num_materials; i++) {
        SDL_ModelMaterial* material = &model->materials[i];
        if (material->diffuse) {
            SDL_ReleaseGPUTexture(model->device, material->diffuse);
        }
        if (material->specular) {
            SDL_ReleaseGPUTexture(model->device, material->specular);
        }
    }

    SDL_free(model);
}