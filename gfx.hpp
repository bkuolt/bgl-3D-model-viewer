// Copyright 2020 Bastian Kuolt
#ifndef GFX_HPP_
#define GFX_HPP_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>  // SDL_GLContext, SDL_Window

#include <ostream>
#include <memory>
#include <filesystem>

#include <glm/glm.hpp>

namespace bgl {

/* ---------------------------- Basic Types ---------------------------- */

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using mat4 = glm::mat4;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoords;
};

using SharedWindow = std::shared_ptr<SDL_Window>;      // TODO(bkuolt): rename
using SharedContext = std::shared_ptr<SDL_GLContext>;  // TODO(bkuolt): rename

/* --------------------------------------------------------------------- */

SharedWindow createFullScreenWindow();
SharedContext createGLContext(const SharedWindow &window);

/* --------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream &os, const vec2 &vector);

#ifdef __linux
namespace console_color {
    constexpr char blue[] = "\x1B[34m";
    constexpr char red[] = "\x1B[31m";
    constexpr char white[] = "\x1B[37m";
    constexpr char magenta[] = "\x1B[35m";
    constexpr char yellow[] = "\x1B[33m";
    constexpr char green[] =  "\x1B[32m";
}  // namespace console_color

#endif  // namespace __linux

/* --------------------------------------------------------------------- */

class Shader {
 public:
    const GLuint _handle;
    const GLuint _type;

    explicit Shader(GLenum type, const std::filesystem::path & path);
    Shader(const Shader&) = delete;
    virtual ~Shader() noexcept;

    Shader& operator=(const Shader&) = delete;

 private:
    void load(const std::filesystem::path & path);
    void compile();
};

using SharedShader = std::shared_ptr<Shader>;


class Program {
 public:
    Program(const SharedShader &vs, const SharedShader &fs_shader);
    Program(const Program&) = delete;
    virtual ~Program() noexcept;

    Program& operator=(const Program&) = delete;

    const GLuint _handle;
 private:
    void link();

    SharedShader _vs;
    SharedShader _fs;
};

using SharedProgram = std::shared_ptr<Program>;
using SharedTexture = std::shared_ptr<GLuint>;  // TODO(bkuolt): implement

/* ------------------------------- Buffer  ------------------------------*/

template<typename T, GLenum type>
class Buffer {
 public:
    Buffer();
    explicit Buffer(GLsizei size);
    virtual ~Buffer() noexcept;

    void bind();

    void resize(GLsizei size);
    GLsizei size() const noexcept;

    T* map();
    void unmap();

 private:
    GLuint _handle { 0 };
    GLsizei _size { 0 };
};

template<typename T, GLenum type>
Buffer<T, type>::Buffer() {
    glCreateBuffers(1, &_handle);
    if (glGetError() != GL_NO_ERROR || _handle == 0) {
        throw std::runtime_error { "could not create GL buffer" };
    }
}

template<typename T, GLenum type>
Buffer<T, type>::Buffer(GLsizei size)
    : Buffer() {
    resize(size);
}

template<typename T, GLenum type>
Buffer<T, type>::~Buffer() noexcept {
    glDeleteBuffers(1, &_handle);
}

template<typename T, GLenum type>
void Buffer<T, type>::bind() {
    glBindBuffer(type, _handle);
}

template<typename T, GLenum type>
void Buffer<T, type>::resize(GLsizei size) {
    bind();
    glBufferData(type, size * sizeof(T), nullptr, GL_STREAM_DRAW);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not resize buffer" };
    }
    _size = size;
}

template<typename T, GLenum type>
GLsizei Buffer<T, type>::size() const noexcept {
    return _size;
}

template<typename T, GLenum type>
T* Buffer<T, type>::map() {
    bind();
    T *buffer = reinterpret_cast<T*>(glMapBuffer(type, GL_WRITE_ONLY));
    if (buffer == nullptr) {
        throw std::runtime_error { "could not create buffer" };
    }
    return buffer;
}

template<typename T, GLenum type>
void Buffer<T, type>::unmap() {
    glUnmapBuffer(type);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not unmap vbo" };
    }
}

using VertexBuffer = Buffer<Vertex, GL_ARRAY_BUFFER>;
using IndexBuffer  = Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>;

using SharedVBO = std::shared_ptr<VertexBuffer>;
using SharedIBO = std::shared_ptr<IndexBuffer>;
using SharedVAO = std::shared_ptr<GLuint>;

/* ------------------------------ Model  -------------------------------- */
struct Model {
    const SharedVBO vbo;
    const SharedIBO ibo;
    const SharedVAO vao;
    const GLsizei num_triangles;

    const SharedTexture texture;
    const SharedProgram program;
};

/* ----------------------------- Interface  ----------------------------- */

using SharedModel = std::shared_ptr<Model>;
SharedModel LoadModel(const std::filesystem::path &path);
void RenderModel(const SharedModel &model, const mat4 &MVP);

}  // namespace bgl

#endif  // GFX_HPP_
