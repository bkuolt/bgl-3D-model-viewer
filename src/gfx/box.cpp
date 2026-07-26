#include "gfx/box.hpp"

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <algorithm>
#include <array>
#include <iostream>

#include "gfx/gfx.hpp"
#include "gfx/gl.hpp"

namespace bgl {

namespace {

constexpr std::array<vec3, 8> box_vertices{{
    {-0.5f, -0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},
    {0.5f, -0.5f, 0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f},
    {0.5f, 0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},
}};

constexpr std::array<uvec2, 12> box_indices{{
    {0, 3},
    {1, 2},
    {0, 1},
    {2, 3},  // front face
    {4, 7},
    {5, 6},
    {4, 5},
    {6, 7},  // back face
    {0, 4},
    {1, 5},
    {3, 7},
    {2, 6}  // side connections
}};

}  // anonymous namespace

Box::Box() {
    _meshes = std::vector<Mesh>(1);

    // Create VBO
    _meshes[0]._vbo.bind();
    _meshes[0]._vbo.allocate(box_vertices.size() * sizeof(vec3));
    auto* vertex_ptr =
        reinterpret_cast<vec3*>(_meshes[0]._vbo.map(QOpenGLBuffer::Access::WriteOnly));
    if (vertex_ptr != nullptr) {
        std::copy(box_vertices.begin(), box_vertices.end(), vertex_ptr);
        _meshes[0]._vbo.unmap();
    }
    _meshes[0]._vbo.release();

    // Create IBO
    _meshes[0]._ibo.bind();
    _meshes[0]._ibo.allocate(box_indices.size() * 2 * sizeof(GLuint));
    auto* index_ptr =
        reinterpret_cast<uvec2*>(_meshes[0]._ibo.map(QOpenGLBuffer::Access::WriteOnly));
    if (index_ptr != nullptr) {
        std::copy(box_indices.begin(), box_indices.end(), index_ptr);
        _meshes[0]._ibo.unmap();
    }
    _meshes[0]._ibo.release();

    // Create VAO
    _program = LoadProgram("./assets/shaders/wireframe.vs", "./assets/shaders/wireframe.fs");
    _program->bind();
    _meshes[0]._vao.bind();
    _meshes[0].bind();

    set_va_attribute(_program->attributeLocation("position"), 3, GL_FLOAT, 0, 0);

    _meshes[0].release();
    _meshes[0]._vao.release();
    _program->release();
}

Box::Box(const BoundingBox& boundingBox) : Box() {
    _boundingBox = boundingBox;
}

void Box::render(const mat4& VP) {
    if (!_program) {
        return;
    }

    _program->bind();
    glLineWidth(3.0f);

    const mat4 M = glm::scale(_boundingBox.getSize());
    const QMatrix4x4 matrix(glm::value_ptr(VP * M));
    _program->setUniformValue("MVP", matrix.transposed());

    constexpr vec3 color{1.0f, 0.0f, 0.0f};  // Red bounding box
    _program->setUniformValue("color", color.x, color.y, color.z);

    _meshes[0].render(GL_LINES);
    _program->release();
}

}  // namespace bgl
