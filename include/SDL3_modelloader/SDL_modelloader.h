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
typedef enum SDL_MDLFormat
{
    SDL_MDLFORMAT_UNKNOWN,
    SDL_MDLFORMAT_OBJ,
    SDL_MDLFORMAT_GLTF,
} SDL_MDLFormat;

/**
 * @brief
 * 
 */
typedef struct SDL_MDLVertex
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
} SDL_MDLVertex;

/**
 * @brief
 * 
 */
typedef struct SDL_MDLMesh
{
    SDL_GPUTexture* diffuse;
    SDL_GPUTexture* specular;
    SDL_GPUBuffer* vertices;
    SDL_GPUBuffer* indices;
    Uint32 num_vertices;
    Uint32 num_indices;
} SDL_MDLMesh;

/**
 * @brief
 * 
 */
typedef struct SDL_MDLModel
{
    SDL_MDLMesh* meshes;
    Uint32 num_meshes;
    SDL_GPUDevice* device;
} SDL_MDLModel;

/**
 * @brief
 * 
 * @param device
 * @param path
 * @param format
 * @return
 */
SDL_DECLSPEC SDL_MDLModel* SDLCALL SDL_MDLLoad(SDL_GPUDevice* device, const char* path, SDL_MDLFormat format);

/**
 * @brief
 * 
 * @param model
 */
SDL_DECLSPEC void SDLCALL SDL_MDLDestroy(SDL_MDLModel* model);

#ifdef __cplusplus
} /* extern "C" */
#endif
#include <SDL3/SDL_close_code.h>

#endif /* ifndef SDL_MODELLOADER_H_ */