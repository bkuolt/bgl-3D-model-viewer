// Copyright 2020 Bastian Kuolt
#include "gfx.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <stdexcept>

#include <assimp/cimport.h>      // aiPropertyStore
#include <assimp/scene.h>        // Output data structure
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/Importer.hpp>   // Model loader

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

shared_context create_GL_context(const shared_window &window) {
    const auto error = glewInit();
    if (GLEW_OK != error) {
        throw std::runtime_error { reinterpret_cast<const char*>(glewGetErrorString(error)) };
    }

    auto context = SDL_GL_CreateContext(window.get());
    if (context == nullptr) {
        throw std::runtime_error { "could not create OpenGL context" };
    }

    const auto Deleter = [] (SDL_GLContext *context) { SDL_GL_DeleteContext(context); };
    return std::shared_ptr<SDL_GLContext>(new SDL_GLContext { context }, Deleter);
}

/* ----------------------- Rendering ----------------------- */

namespace {

std::shared_ptr<GLuint> create_buffer() {
    GLuint buffer = 0;
    glCreateBuffers(1, &buffer);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not create GL buffer" };
    }

    return { new GLuint { buffer  },
        [] (GLuint *buffer) {
            glDeleteBuffers(1, buffer);
            delete buffer;
        }
    };
}

shared_vbo create_vbo(const shared_model &model) {
    auto vbo = create_buffer();
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    // TODO(bkuolt): fill buffer
    return vbo;
}

shared_ibo create_ibo(const shared_model &model) {
    auto ibo = create_buffer();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ibo);
    // TODO(bkuolt): fill buffer
    return ibo;
}

shared_vao create_vao(const shared_model &model) {
    GLuint vao = 0;
    __glewGenVertexArrays(1, &vao);
    return {};  // TODO(bkuolt)
}

}  // namespace

std::shared_ptr<model> load_model(const std::filesystem::path &path) {
    Assimp::Importer importer;


    aiPropertyStore* props = aiCreatePropertyStore();
    aiSetImportPropertyInteger(props, "PP_PTV_NORMALIZE", 1);
    auto scene = aiImportFileExWithProperties(path.string().c_str(),
        aiProcess_Triangulate            |
        aiProcess_GenSmoothNormals |
        aiProcess_GenNormals |
        aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices,
        nullptr,
        props);
    aiReleasePropertyStore(props);

    if (scene == nullptr) {
        throw std::runtime_error(importer.GetErrorString());
    }

    [[maybe_unused]] const auto &mesh = scene->mMeshes[0];
    // TODO(bkuolt): create vbo
    // TODO(bkuolt): create ibo
    // TODO(bkuolt): create vao
    // TODO(bkuolt): load texture from Assimp and convert it to GL

    return {};
}