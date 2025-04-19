#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_modelloader/SDL_modelloader.h>
#include <stddef.h> /* offsetof */
#include "HandmadeMath.h"

static SDL_Window* window;
static SDL_GPUDevice* device;
static SDL_GPUGraphicsPipeline* pipeline;
static SDL_GPUTexture* depth_texture;
static Uint32 width;
static Uint32 height;
static HMM_Vec3 position;
static float pitch;
static float yaw;
static float zoom;

static SDL_GPUShader* LoadShader(const char* path, const int uniforms, const int samplers)
{
    SDL_GPUShaderCreateInfo info = {0};
    void* code = SDL_LoadFile(path, &info.code_size);
    if (!code) {
        SDL_Log("Failed to load shader: %s, %s", path, SDL_GetError());
        return NULL;
    }

    info.code = code;
    info.num_uniform_buffers = uniforms;
    info.num_samplers = samplers;
    info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    info.entrypoint = "main";
    if (SDL_strstr(path, ".vert")) {
        info.stage = SDL_GPU_SHADERSTAGE_VERTEX;
    } else {
        info.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    }

    SDL_GPUShader* shader = SDL_CreateGPUShader(device, &info);
    if (!shader) {
        SDL_Log("Failed to create shader: %s, %s", path, SDL_GetError());
        return NULL;
    }
    SDL_free(code);
    return shader;
}

static SDL_GPUGraphicsPipeline* LoadPipeline()
{
    SDL_GPUGraphicsPipelineCreateInfo info = {0};
    info.vertex_shader = LoadShader("sample.vert", 1, 0);
    info.fragment_shader = LoadShader("sample.frag", 0, 1);
    if (!info.vertex_shader || !info.fragment_shader) {
        SDL_Log("Failed to load graphics pipeline");
        return NULL;
    }

    info.vertex_input_state = (SDL_GPUVertexInputState) {
        .num_vertex_attributes = 3,
        .vertex_attributes = (SDL_GPUVertexAttribute[]) {{
            .location = 0,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = offsetof(SDL_ModelVertex, position),
        }, {
            .location = 1,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
            .offset = offsetof(SDL_ModelVertex, uv),
        }, {
            .location = 2,
            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset = offsetof(SDL_ModelVertex, normal),
        }},
        .num_vertex_buffers = 1,
        .vertex_buffer_descriptions = (SDL_GPUVertexBufferDescription[]) {{
            .pitch = sizeof(SDL_ModelVertex),
        }},
    };

    info.target_info = (SDL_GPUGraphicsPipelineTargetInfo) {
        .num_color_targets = 1,
        .color_target_descriptions = (SDL_GPUColorTargetDescription[]) {{
            .format = SDL_GetGPUSwapchainTextureFormat(device, window),
        }},
        .has_depth_stencil_target = true,
        .depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
    };

    info.depth_stencil_state = (SDL_GPUDepthStencilState) {
        .enable_depth_test = true,
        .enable_depth_write = true,
        .compare_op = SDL_GPU_COMPAREOP_LESS,
    };

    info.rasterizer_state = (SDL_GPURasterizerState) {
        .cull_mode = SDL_GPU_CULLMODE_BACK,
        .front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
        .fill_mode = SDL_GPU_FILLMODE_FILL,
    };

    SDL_GPUGraphicsPipeline* pipeline = SDL_CreateGPUGraphicsPipeline(device, &info);
    if (!pipeline) {
        SDL_Log("Failed to create graphics pipeline: %s", SDL_GetError());
        return NULL;
    }
    SDL_ReleaseGPUShader(device, info.vertex_shader);
    SDL_ReleaseGPUShader(device, info.fragment_shader);
    return pipeline;
}

static SDL_GPUTexture* CreateDepthTexture(const Uint32 width, const Uint32 height)
{
    SDL_GPUTextureCreateInfo info = {0};
    info.type = SDL_GPU_TEXTURETYPE_2D;
    info.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
    info.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    info.width = width;
    info.height = height;
    info.layer_count_or_depth = 1;
    info.num_levels = 1;
    SDL_GPUTexture* texture = SDL_CreateGPUTexture(device, &info);
    if (!texture) {
        SDL_Log("Failed to create depth texture: %s", SDL_GetError());
        return NULL;
    }
    return texture;
}

static void Draw()
{
    SDL_WaitForGPUSwapchain(device, window);

    SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(device);
    if (!command_buffer) {
        SDL_Log("Failed to acquire command buffer: %s", SDL_GetError());
        return;
    }

    SDL_GPUTexture* color_texture;
    Uint32 w;
    Uint32 h;
    if (!SDL_AcquireGPUSwapchainTexture(command_buffer, window, &color_texture, &w, &h)) {
        SDL_Log("Failed to acquire swapchain texture: %s", SDL_GetError());
        SDL_CancelGPUCommandBuffer(command_buffer);
        return;
    }

    if (width != w || height != h) {
        if (depth_texture) {
            SDL_ReleaseGPUTexture(device, depth_texture);
        }
        depth_texture = CreateDepthTexture(w, h);
        if (!depth_texture) {
            SDL_SubmitGPUCommandBuffer(command_buffer);
        }
        width = w;
        height = h;
    }

    SDL_SubmitGPUCommandBuffer(command_buffer);
}

int main(int argc, char** argv)
{
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    SDL_SetAppMetadata("SDL_modelloader_sample", NULL, NULL);
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    if (!(window = SDL_CreateWindow("SDL_modelloader_sample", 960, 720, SDL_WINDOW_RESIZABLE))) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return 1;
    }
    if (!(device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL))) {
        SDL_Log("Failed to create device: %s", SDL_GetError());
        return 1;
    }
    if (!SDL_ClaimWindowForGPUDevice(device, window)) {
        SDL_Log("Failed to create swapchain: %s", SDL_GetError());
        return 1;
    }
    if (!(pipeline = LoadPipeline())) {
        SDL_Log("Failed to load pipeline");
        return 1;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_DROP_FILE:
                break;
            }
        }

        Draw();
    }

    SDL_ReleaseGPUTexture(device, depth_texture);
    SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
    SDL_ReleaseWindowFromGPUDevice(device, window);
    SDL_DestroyGPUDevice(device);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}