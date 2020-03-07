// Copyright 2020 Bastian Kuolt
#include "gfx.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include <string>

#include <assimp/scene.h>        // Output data structure
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/Importer.hpp>  // Model loader

namespace {

void initialize_SDL() {
    static bool initialized = false;

    if (!initialized) {
        const auto result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        if (result == -1) {
            throw std::runtime_error { "could not initialize SDL2" };
        }
        std::atexit(SDL_Quit);
        initialized = true;
    }
}

}  // namespace


std::shared_ptr<SDL_Window> create_fullscreen_window() {
    initialize_SDL();
    SDL_Window * const window = SDL_CreateWindow("BGL Tech Demo",
                                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                                 800, 600,
                                                 SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        throw std::runtime_error{ SDL_GetError() };
    }
    const auto Deleter = [] (SDL_Window *window) { SDL_DestroyWindow(window); };
    return std::shared_ptr<SDL_Window>(window, Deleter);
}

std::shared_ptr<SDL_Renderer> create_renderer(const std::shared_ptr<SDL_Window> &window) {
    auto renderer = SDL_CreateRenderer(window.get(),
                                     -1,
                                      0 /* TODO(bkuol): set flags */);
    if (renderer == nullptr) {
        throw std::runtime_error{ SDL_GetError() };
    }
    const auto Deleter = [] (SDL_Renderer *renderer) { SDL_DestroyRenderer(renderer); };
    return std::shared_ptr<SDL_Renderer>(renderer, Deleter);
}

/* ----------------------- Rendering ----------------------- */

std::shared_ptr<model> load_model(const std::filesystem::path &path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string().c_str(),
                                             aiProcess_CalcTangentSpace       |
                                             aiProcess_Triangulate            |
                                             aiProcess_JoinIdenticalVertices  |
                                             aiProcess_SortByPType);
    if (scene == nullptr) {
        throw std::runtime_error(importer.GetErrorString());
    }

    const auto &mesh = scene->mMeshes[0];
    // TODO(bkuolt): create vbo
    // TODO(bkuolt): create ibo
    // TODO(bkuolt): create vao
    // TODO(bkuolt): load texture from Assimp and convert it to GL

    return {};
}