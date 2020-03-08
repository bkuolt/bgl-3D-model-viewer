// Copyright 2020 Bastian Kuolt
#include "gfx.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>

#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>

#include <assimp/cimport.h>      // aiPropertyStore
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/Importer.hpp>   // Model loader
#include <assimp/scene.h>        // Output data structure
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

/* ----------------------- Shader Support  ----------------------- */

namespace {

void load_shader_source(GLuint handle, const std::filesystem::path &path) {
    // TODO(bkuolt): implement
}

SharedShader LoadShader(GLenum type, const std::filesystem::path &path) {
    GLuint handle = glCreateShader(type);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "" /* TODO(bkuolt*/ };
    }

    constexpr auto Deleter = [] (GLuint *pointer) {
        glDeleteShader(*pointer);
        delete pointer;
    };
    auto shader = std::shared_ptr<GLuint> { new GLuint { handle }, Deleter };
    load_shader_source(handle, path);
    glCompileShader(handle);
    // TODO(bkuolt): check compile status
    return shader;
}

SharedProgram LoadProgram(const std::filesystem::path &vs_path, const std::filesystem::path &fs_path) {
    auto vs = LoadShader(GL_VERTEX_SHADER, vs_path);
    auto fs = LoadShader(GL_FRAGMENT_SHADER, fs_path);

    GLuint handle = glCreateProgram();
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "" /* TODO(bkuolt*/ };
    }

    glAttachShader(handle, *vs);
    glAttachShader(handle, *fs);
    glLinkProgram(handle);
    // TODO(bkuolt): check for errors

    constexpr auto Deleter = [] (GLuint *pointer) {
        glDeleteProgram(*pointer);
        delete pointer;
    };
    return { new GLuint { handle }, Deleter };
}


// TODO(bkuolt): Compile()
// TODO(bkuolt): Link()
// TODO(bkuolt): LoadShaderSource()
// TODO(bkuolt): LoadShader()

}  // namespace

/* ------------------------- Rendering --------------------------- */

namespace {

std::shared_ptr<GLuint> create_buffer() {
    GLuint buffer = 0;
    glCreateBuffers(1, &buffer);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not create GL buffer" };
    }

    return { &buffer,
        [] (GLuint *buffer) {
            glDeleteBuffers(1, buffer);
        }
    };
}

shared_vbo create_vbo(const aiMesh &mesh) {
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 texcoords;
    };

    auto vbo = create_buffer();
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);

    const GLsizei size = sizeof(Vertex) * mesh.mNumVertices;
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);
    Vertex *buffer = reinterpret_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    if (buffer == nullptr) {
        throw std::runtime_error { "could not create vbo" };
    }

    static_assert(std::is_same<ai_real, float>::value);
    for (auto i = 0u; i < mesh.mNumVertices; ++i) {
        buffer[i].normal = vec3 { mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z };
        buffer[i].position = vec3 { mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z };
        buffer[i].texcoords = vec2 { mesh.mTextureCoords[0][i].x, mesh.mTextureCoords[0][i].y };
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not unmap vbo" };
    }

    return vbo;
}

shared_ibo create_ibo(const aiMesh &mesh) {
    auto ibo = create_buffer();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ibo);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not write data to VBO" };
    }

    const GLsizei size = sizeof(GLuint) * mesh.mNumFaces * 30;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STREAM_DRAW);
    auto buffer = reinterpret_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
    if (buffer == nullptr) {
        throw std::runtime_error { "could not create VBO" };
    }

    for (auto i = 0u; i < mesh.mNumFaces; ++i) {
        switch (mesh.mFaces[i].mNumIndices) {
            case 3:
                std::copy_n(mesh.mFaces[i].mIndices, 3, buffer);
                buffer += 3;
                break;
            default:
                glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
                throw std::runtime_error { "unexpected data" };
        }
    }

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not write data to VBO" };
    }

    std::cout << "created vbo for " << mesh.mNumFaces * 3 << " indices" << std::endl;
    return ibo;
}

shared_vao create_vao(const shared_vbo vbo, shared_ibo ibo) {
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    return {};  // TODO(bkuolt): implement
}

std::shared_ptr<GLuint> create_texture() {
    return {};  // TODO(bkuolt): implement
}


}  // namespace

SharedModel LoadModel(const std::filesystem::path &path) {
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

    const auto vbo = create_vbo(mesh);
    const auto ibo = create_ibo(mesh);
    Model model;

    return std::make_shared<Model>( Model {
        .vbo = vbo, .ibo = ibo,
        .vao = create_vao(vbo, ibo),
        .texture = create_texture(),
        .program = LoadProgram("/assets/main.vs", "/assets/main.fs"),
        .vertex_count = static_cast<GLsizei>(mesh.mNumVertices)
    });
}

void render_model(const SharedModel &model) {
    // TODO(bkuolt): bind vbo
    // TODO(bkuolt): bind vao
    // TODO(bkuolt): set uniforms
    glUseProgram(*model->program);
    glDrawElements(GL_TRIANGLES, model->vertex_count * 3, GL_UNSIGNED_INT, nullptr);
}
