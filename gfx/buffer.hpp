// Copyright 2020 Bastian Kuolt
#ifndef GFX_BUFFER_HPP_
#define GFX_BUFFER_HPP_

#include "gl.hpp"
#include <memory>
#include <iomanip>  // std::quoted
#include <stdexcept>

namespace bgl {

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoords;
};

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
    if (buffer == nullptr || glGetError() != GL_NO_ERROR) {
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

class VertexArray {
 public:
    VertexArray(const SharedVBO &vbo, const SharedIBO &ibo)
        : _vbo(vbo), _ibo(ibo) {
        glGenVertexArrays(1, &_handle);
        if (_handle == 0 || glGetError() != GL_NO_ERROR) {
            throw std::runtime_error { "could not create vertex array" };
        }

        glBindVertexArray(_handle);
        _vbo->bind();
        _ibo->bind();
        unbind();
    }

    VertexArray(VertexArray&&) = default;
    VertexArray& operator=(VertexArray&&) = default;

    void bind() noexcept {
        glBindVertexArray(_handle);
    }

    void unbind() noexcept {
        glBindVertexArray(0);
    }

    void setAttribute(GLuint location, GLenum type, GLsizei size, GLsizei stride, GLsizei offset) {
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, size, type, GL_FALSE, stride, reinterpret_cast<void*>(offset));
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error { "glVertexAttribPointer() failed" };
        }
    }

 private:
    GLuint _handle { 0 };
    const SharedVBO _vbo;
    const SharedIBO _ibo;
};

using SharedVAO = std::shared_ptr<VertexArray>;


template<typename T>
void SetAttribute(const SharedVAO &vao, GLuint location, GLsizei stride, GLsizei offset);

template<>
inline void SetAttribute<GLuint>(const SharedVAO &vao, GLuint location, GLsizei stride, GLsizei offset) {
    vao->setAttribute(location, GL_UNSIGNED_INT, 1, stride, offset);
}

template<>
inline void SetAttribute<GLfloat>(const SharedVAO &vao, GLuint location, GLsizei stride, GLsizei offset) {
    vao->setAttribute(location, GL_FLOAT, 1, stride, offset);
}

template<>
inline void SetAttribute<vec3>(const SharedVAO &vao, GLuint location, GLsizei stride, GLsizei offset) {
    vao->setAttribute(location, GL_FLOAT, 3, stride, offset);
}

template<>
inline void SetAttribute<vec2>(const SharedVAO &vao, GLuint location, GLsizei stride, GLsizei offset) {
    vao->setAttribute(location, GL_FLOAT, 2, stride, offset);
}

}  // namespace bgl

#endif  // GFX_BUFFER_HPP_
