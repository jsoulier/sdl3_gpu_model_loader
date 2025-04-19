#ifndef SDL_MODELLOADER_H_
#define SDL_MODELLOADER_H_

#include <SDL3/SDL.h>
#include <SDL3/SDL_begin_code.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * 
 */
typedef enum SDL_ModelFormat
{
    SDL_ModelFORMAT_UNKNOWN,
    SDL_ModelFORMAT_OBJ,
    SDL_ModelFORMAT_GLTF,
} SDL_ModelFormat;

/**
 * @brief
 * 
 */
typedef struct SDL_ModelVertex
{
    struct {
        float x;
        float y;
        float z;
    } position;
    struct {
        float x;
        float y;
    } uv;
    struct {
        float x;
        float y;
        float z;
    } normal;
} SDL_ModelVertex;

typedef struct SDL_ModelMaterial
{
    SDL_GPUTexture* diffuse;
    SDL_GPUTexture* specular;
} SDL_ModelMaterial;

/**
 * @brief
 * 
 */
typedef struct SDL_ModelMesh
{
    SDL_GPUBuffer* vertices;
    SDL_GPUBuffer* indices;

    Uint32 num_vertices;
    Uint32 num_indices;

    Uint32 material;
} SDL_ModelMesh;

/**
 * @brief
 * 
 */
typedef struct SDL_Model
{
    SDL_ModelMesh* meshes;
    Uint32 num_meshes;

    SDL_ModelMaterial* materials;
    Uint32 num_materials;

    SDL_GPUDevice* device;
} SDL_Model;

/**
 * @brief
 * 
 * @param device
 * @param path
 * @param format
 * @return
 */
SDL_DECLSPEC SDL_Model* SDLCALL SDL_ModelLoad(SDL_GPUDevice* device, const char* path, SDL_ModelFormat format);

/**
 * @brief
 * 
 * @param model
 */
SDL_DECLSPEC void SDLCALL SDL_ModelDestroy(SDL_Model* model);

#ifdef __cplusplus
} /* extern "C" */
#endif
#include <SDL3/SDL_close_code.h>

#endif /* ifndef SDL_MODELLOADER_H_ */