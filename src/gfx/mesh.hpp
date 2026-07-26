#pragma once

/**
 * @file mesh.hpp
 * @brief OpenGL Mesh data structures and buffer management.
 */

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <optional>

#include "gfx/gl.hpp"

namespace bgl {

/**
 * @brief Vertex layout for VBO memory allocations.
 */
struct Vertex {
    vec3 position{0.0f};
    vec3 normal{0.0f};
    vec2 texcoords{0.0f};
};

/**
 * @brief Manages OpenGL buffer objects (VBO, IBO, VAO) for a single mesh chunk.
 */
struct Mesh {
    Mesh();
    ~Mesh() noexcept = default;

    Mesh(Mesh&&) noexcept = default;
    Mesh& operator=(Mesh&&) noexcept = default;

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void bind();
    void release();

    void render(GLenum mode, GLuint count);
    void render(GLenum mode);

    QOpenGLBuffer _vbo;
    QOpenGLBuffer _ibo;
    QOpenGLVertexArrayObject _vao;
    std::optional<unsigned int> _materialIndex;
};

}  // namespace bgl
