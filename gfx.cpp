// Copyright 2020 Bastian Kuolt
#include "gfx.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <cassert>
#include <string>
#include <stdexcept>

#include <assimp/cimport.h>      // aiPropertyStore
#include <assimp/scene.h>        // Output data structure
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/Importer.hpp>   // Model loader

#include <type_traits>

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    auto context = SDL_GL_CreateContext(window.get());
    if (context == nullptr) {
        throw std::runtime_error { "could not create OpenGL context" };
    }

    const auto error = glewInit();
    if (GLEW_OK != error) {
        throw std::runtime_error { reinterpret_cast<const char*>(glewGetErrorString(error)) };
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

    return { new GLuint { buffer },
        [] (GLuint *buffer) {
            glDeleteBuffers(1, buffer);
            delete buffer;
        }
    };
}

shared_vbo create_vbo(const aiMesh &mesh) {
    auto vbo = create_buffer();
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);

    // TODO(bkuolt): store texture coordinates
    // TODO(bkuolt): store normals
    // TODO(bkuolt): store positions

    return vbo;
}

shared_ibo create_ibo(const aiMesh &mesh) {
    auto ibo = create_buffer();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ibo);

    const GLsizei size = sizeof(GLuint) * mesh.mNumFaces * 3;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);
    auto buffer = reinterpret_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
    if (buffer == nullptr) {
        throw std::runtime_error { "could not create VBO" };  // TODO(bkuolt)
    }

    for (auto i = 0u; i < mesh.mNumFaces * 3; ++i) {
        switch (mesh.mFaces[i].mNumIndices) {
            case 3:
                std::copy_n(mesh.mFaces[i].mIndices, 3, buffer);
                buffer += 3;
                break;
            case 4:
                buffer += 3;  // ignore last vertes as this is a bug -> see https://github.com/assimp/assimp/issues/3019
                break;
            default:
                std::cout << mesh.mFaces[i].mNumIndices << std::endl;
                //throw std::runtime_error { "unsupported format" };
        }


        buffer += 3;
    }

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not write data to VBO" };
    }

    return ibo;
}

shared_vao create_vao(const shared_vbo vbo, shared_ibo ibo) {
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    return {};  // TODO(bkuolt): implement
}

}  // namespace

std::shared_ptr<model> load_model(const std::filesystem::path &path) {
    aiPropertyStore* props = aiCreatePropertyStore();
    if (props == nullptr) {
        throw std::runtime_error { aiGetErrorString() };
    }

    aiSetImportPropertyInteger(props, AI_CONFIG_PP_PTV_NORMALIZE, 1);
    auto scene = aiImportFileExWithProperties(path.string().c_str(),
        aiProcess_Triangulate            |
        aiProcess_GenSmoothNormals |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices,
        nullptr,
        props);
    aiReleasePropertyStore(props);

    if (scene == nullptr) {
        throw std::runtime_error { aiGetErrorString() };
    }

    const aiMesh &mesh = *scene->mMeshes[0];
    auto vbo = create_vbo(mesh);
    auto ibo = create_ibo(mesh);
    auto vao = create_vao(vbo, ibo);
    // TODO(bkuolt): load texture from Assimp and convert it to GL

    return {};
}

void render_model(const shared_model &model) {
    // TODO(bkuolt)
}
