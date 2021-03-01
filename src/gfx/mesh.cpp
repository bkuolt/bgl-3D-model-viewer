// Copyright 2021 Bastian Kuolt
#include "mesh.hpp"

#include <iostream>
#include <stdexcept>


namespace bgl {

Mesh::Mesh()
    : _vbo { QOpenGLBuffer::VertexBuffer },
      _ibo { QOpenGLBuffer::IndexBuffer } {
    if (!_vbo.create()) {
        throw std::runtime_error { "could not create VBO" };
    }
    if (!_ibo.create()) {
        throw std::runtime_error { "could not create IBO" };
    }
    if (!_vao.create()) {
        throw std::runtime_error { "could not create VAO" };
    }
}

void Mesh::bind() {
    _vao.bind();
    _vbo.bind();
    _ibo.bind();
}

// TODO: Release

void Mesh::render(GLenum mode, GLuint count) {
    bind();
    glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "glDrawElements() failed" };
    }

    _vbo.release();
    _ibo.release();
    _vao.release();
}

void Mesh::render(GLenum mode) {
    _ibo.bind();  // for _ibo.size()
    render(mode, _ibo.size() / sizeof(GLuint));
}

}  // namespace bgl
