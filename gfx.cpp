// Copyright 2020 Bastian Kuolt
#include "gfx.hpp"

#include <GL/glu.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>

#include <algorithm>  // std::for_each
#include <iomanip>    // std::setprecision, std::fixed
#include <iostream>
#include <fstream>

#include <sstream>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include <assimp/cimport.h>      // aiPropertyStore
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/Importer.hpp>   // Model loader
#include <assimp/scene.h>        // Output data structure

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>  // glm::value_ptr()


namespace bgl {

std::ostream& operator<<(std::ostream &os, const vec2 &vector) {
    os << "(" << std::fixed << std::setprecision(2) << vector.x
       << " | "
       << std::fixed << std::setprecision(2) << vector.y << ")";
    return os;
}

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

std::shared_ptr<SDL_Window> createFullScreenWindow() {
    initialize_SDL();
    SDL_Window * const window = SDL_CreateWindow("BGL Tech Demo",
                                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                                 1280, 720,
                                                 SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN /* | SDL_WINDOW_FULLSCREEN_DESKTOP */);
    if (window == nullptr) {
        throw std::runtime_error{ SDL_GetError() };
    }

    const auto Deleter = [] (SDL_Window *window) { SDL_DestroyWindow(window); };
    return std::shared_ptr<SDL_Window>(window, Deleter);
}

SharedContext createGLContext(const SharedWindow &window) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    auto context = SDL_GL_CreateContext(window.get());
    if (context == nullptr) {
        throw std::runtime_error { "could not create OpenGL context" };
    }

    const auto error = glewInit();
    if (GLEW_OK != error) {
        throw std::runtime_error { reinterpret_cast<const char*>(glewGetErrorString(error)) };
    }

    std::cout << "GL: "<< glGetString(GL_VERSION)
              << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    const auto Deleter = [] (SDL_GLContext *context) { SDL_GL_DeleteContext(context); };
    return std::shared_ptr<SDL_GLContext>(new SDL_GLContext { context }, Deleter);
}

/* ----------------------- Shader Support  ----------------------- */

Shader::Shader(GLenum type, const std::filesystem::path & path)
    : _handle { glCreateShader(type) }, _type { type } {
    if (_handle == 0) {
        throw std::runtime_error { "could not create shader object"};
    }
    load(path);
    compile();
}

Shader::~Shader() noexcept {
    glDeleteShader(_handle);
}

void Shader::load(const std::filesystem::path &path) {
    std::vector<GLchar*> lines;

    std::ifstream file { path.string() };
    std::string string;
    std::getline(file, string);
    while (!file.eof()) {
        if (file.fail()) {
            throw std::runtime_error { "could not read shader source file " };
        }

        GLchar *line = strdup((std::string(string.c_str()) + "\n").c_str());
        lines.push_back(line);
        // std::cout << std::quoted(line) << std::endl;
        std::getline(file, string);
    }

    glShaderSource(_handle, lines.size(), lines.data(), nullptr);
    std::for_each(lines.begin(), lines.end(),  [] (GLchar *line) { free(line); });
    std::cout << "loaded shader source " << path << std::endl;
}

void Shader::compile() {
    glCompileShader(_handle);

    GLint isCompiled = 0;
    glGetShaderiv(_handle, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
        GLint maxLength = 0;
        glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &maxLength);
        std::basic_string<GLchar> errorLog;
        errorLog.resize(maxLength);
        glGetShaderInfoLog(_handle, maxLength, nullptr, errorLog.data());
        throw std::runtime_error { "shader compile log: " + errorLog };
    }
    std::cout << "successfully compiled shader handle=" << _handle << std::endl;
}


Program::Program(const SharedShader &vs, const SharedShader &fs)
    : _handle { glCreateProgram() },
      _vs { vs }, _fs { fs } {
    if (_handle == 0) {
        throw std::runtime_error { "could not create shader program" };
    }

    glAttachShader(_handle, _vs->_handle);
    glAttachShader(_handle, _fs->_handle);
    link();
}

Program::~Program() noexcept {
    glDeleteProgram(_handle);
}

void Program::link() {
    glLinkProgram(_handle);

    GLint isLinked = 0;
    glGetProgramiv(_handle, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &maxLength);
        std::basic_string<GLchar> errorLog;
        errorLog.resize(maxLength);
        glGetProgramInfoLog(_handle, maxLength, &maxLength, errorLog.data());
        throw std::runtime_error { "shader link log: " + errorLog };
    }
    std::cout << "linked shader program" << std::endl;
}

namespace {

inline SharedShader LoadShader(GLenum type, const std::filesystem::path &path) {
    return std::make_shared<Shader>(type, path);
}

}  // namespace

/* ------------------------- Rendering --------------------------- */

namespace {



struct bounding_box {
    float min;
    float max;
};

glm::tvec3<bounding_box> get_bounds(const aiMesh &mesh) noexcept {
    glm::tvec3<bounding_box> bounds;

    for (auto i = 0u; i < 3; ++i) {
        bounds[i].min = 0;
        bounds[i].max = 0;
    }

    for (auto vertex_index = 0u; vertex_index < mesh.mNumVertices; ++vertex_index) {
        for (auto i = 0u; i < 3; ++i) {
            bounds[i].min = std::min(bounds[i].min, mesh.mVertices[vertex_index][i]);
            bounds[i].max = std::max(bounds[i].max, mesh.mVertices[vertex_index][i]);
        }
    }
    return bounds;
}

void normalize_vertex_positions(const aiMesh &mesh, Vertex *buffer) {
    const glm::tvec3<bounding_box> bounds = get_bounds(mesh);
    const vec3 dimensions {
        bounds.x.max - bounds.x.min,
        bounds.y.max - bounds.y.min,
        bounds.z.max - bounds.z.min,
    };
    const vec3 center {
        bounds.x.max - (dimensions.x / 2),
        bounds.y.max - (dimensions.y / 2),
        bounds.z.max - (dimensions.z / 2)
    };

    std::cout << "dimensions:"
              << " w=" << dimensions.x
              << " h=" << dimensions.y
              << " d=" << dimensions.z
              << std::endl;

    for (auto vertex_index = 0u; vertex_index < mesh.mNumVertices; ++vertex_index) {
        for (auto i = 0u; i < 3; ++i) {
            buffer[vertex_index].position[i] -= center[i];      // recenter
            buffer[vertex_index].position[i] /= dimensions[i];  // normalize
        }
    }
}

SharedVBO createVBO(const aiScene *scene) {
    static_assert(std::is_same<ai_real, float>::value);
    const aiMesh &mesh = *scene->mMeshes[0];

    auto vbo = std::make_shared<VertexBuffer>(mesh.mNumVertices);
    Vertex *buffer = vbo->map();
    const bool is_textured { scene->mNumTextures > 1 };
    for (auto vertex_index = 0u; vertex_index < mesh.mNumVertices; ++vertex_index) {
        buffer[vertex_index].normal = -vec3 { mesh.mNormals[vertex_index].x, mesh.mNormals[vertex_index].y, mesh.mNormals[vertex_index].z };
        buffer[vertex_index].position = vec3 { mesh.mVertices[vertex_index].x, mesh.mVertices[vertex_index].y, mesh.mVertices[vertex_index].z };
        if (is_textured) {
            buffer[vertex_index].texcoords = vec2 { mesh.mTextureCoords[0][vertex_index].x,
                                                    mesh.mTextureCoords[0][vertex_index].y };
        }
    }

    normalize_vertex_positions(mesh, buffer);
    vbo->unmap();

    std::cout << "created a vbo with " << vbo->size() << " vertices" << std::endl;
    return vbo;
}

SharedIBO createIBO(const aiMesh &mesh) {
    const GLsizei size = mesh.mNumFaces * 3;
    auto ibo = std::make_shared<IndexBuffer>(size);

    auto buffer = ibo->map();
    for (auto i = 0u; i < mesh.mNumFaces; ++i) {
        switch (mesh.mFaces[i].mNumIndices) {
            case 3:
                std::copy_n(mesh.mFaces[i].mIndices, 3, buffer);
                buffer += 3;
                break;
            default:
                throw std::runtime_error { "unexpected data" };
        }
    }
    ibo->unmap();

    std::cout << "created an ibo with " << ibo->size() << " indices" << std::endl;
    return ibo;
}

enum AttributLocations { MVP = 0, Position = 1, Normal, TexCoord, Texture };

SharedVAO createVAO(const SharedVBO &vbo, const SharedIBO &ibo) {
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    if (vao == 0) {
        throw std::runtime_error { "could not create VAO" };
    }

    glBindVertexArray(vao);

    glEnableVertexAttribArray(AttributLocations::Position);
    glEnableVertexAttribArray(AttributLocations::Normal);
    glEnableVertexAttribArray(AttributLocations::TexCoord);

    const auto stride = sizeof(Vertex);
    vbo->bind();
    glVertexAttribPointer(AttributLocations::Position, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, position)));
    glVertexAttribPointer(AttributLocations::Normal, 3, GL_FLOAT, GL_TRUE, stride, reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glVertexAttribPointer(AttributLocations::TexCoord, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, texcoords)));

    ibo->bind();

    glBindVertexArray(0);
    return std::shared_ptr<GLuint> { new GLuint { vao }, [] (GLuint *pointer) {
                                                               glDeleteVertexArrays(1, pointer);
                                                               delete pointer;
                                                           }};
}

std::shared_ptr<GLuint> create_texture(const aiTexture *texture) {
    std::cout << "loaded texture" << std::endl;
    return {};  // TODO(bkuolt): implement
}

}  // namespace

SharedModel LoadModel(const std::filesystem::path &path) {
    if (!std::filesystem::exists(path)) {
        std::ostringstream oss;
        oss << "The file " << std::quoted(path.string()) << " does not exist";
        throw std::runtime_error { oss.str() };
    }

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
    const auto vbo = createVBO(scene);
    const auto ibo = createIBO(mesh);
    const auto vao = createVAO(vbo, ibo);

    const auto texture { (scene->mNumTextures > 1) ? create_texture(scene->mTextures[0]) : SharedTexture {} };
    if (scene->mNumTextures > 1) {
        std::cout << console_color::yellow << "warning: "
                  << "uses " << scene->mNumTextures << " textures, but only one is supported"
                  << console_color::white << std::endl;
    }

    const auto vs = LoadShader(GL_VERTEX_SHADER, "./assets/main.vs");
    const auto fs = LoadShader(GL_FRAGMENT_SHADER, "./assets/main.fs");
    const auto program = std::make_shared<Program>(vs, fs);

    const Model model { .vbo = vbo, .ibo = ibo, .vao = vao,
                        .num_triangles = static_cast<GLsizei>(mesh.mNumFaces),
                        .texture = texture,
                        .program = program };
    return std::make_shared<Model>(model);
}

void RenderModel(const SharedModel &model, const mat4 &MVP) {
    glUseProgram(model->program->_handle);

    if (model->texture) {
        constexpr GLuint texture_unit = 0;
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, *model->texture);
        glUniform1ui(AttributLocations::Texture, texture_unit);
    }

    glBindVertexArray(*model->vao);
    glUniformMatrix4fv(AttributLocations::MVP, 1, GL_FALSE, glm::value_ptr(MVP));
    glDrawElements(GL_TRIANGLES, model->num_triangles * 3, GL_UNSIGNED_INT, nullptr);
}

}  // namespace bgl
