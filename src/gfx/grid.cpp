// Copyright 2021 Bastian Kuolt
#include "grid.hpp"
#include "gfx.hpp"

#include <QMatrix4x4>


namespace bgl {

enum locations { MVP = 0 , color, position };

Grid::Grid(GLfloat size, std::size_t num_cells)
    : _cell_size { size },
      _num_cells { num_cells } {
    _program = LoadProgram("./assets/shaders/wireframe.vs", "./assets/shaders/wireframe.fs");

    create_vbo();
    create_ibo();
    create_vao();
}

void Grid::create_vbo() {
    auto get_index = [&](int x, int z) -> GLuint { return (_num_cells * z) + x; };

    const float size = _num_cells * _cell_size;
    const vec3 T { size / 2.0f, 0.0f, size / 2.0f };

    _vbo->bind();
    _vbo->allocate(sizeof(vec3) * _num_cells * _num_cells);

    vec3 *buffer = reinterpret_cast<vec3*>(_vbo->map(QOpenGLBuffer::WriteOnly));
    for (auto z = 0u; z < _num_cells; ++z) {
        for (auto x = 0u; x < _num_cells; ++x) {
            buffer[get_index(x, z)] = vec3 { x * _cell_size, 0.0, z * _cell_size } - T;
        }
    }
    _vbo->unmap();
}

void Grid::create_ibo() {
    auto get_index = [&](int x, int z) { return (_num_cells * z) + x; };

    const size_t num_triangles { 2 * _num_cells * _num_cells + 4 };
    _ibo->bind();
    _ibo->allocate(num_triangles * 3 * sizeof(GLuint) /* vertices */);

    using uvec2 = glm::tvec2<GLuint>;
    uvec2 *buffer = reinterpret_cast<uvec2*>(_ibo->map(QOpenGLBuffer::WriteOnly));

    for (auto z = 0u; z < _num_cells - 1; ++z) {
        for (auto x = 0u; x < _num_cells - 1; ++x) {
            *buffer++ = uvec2 { get_index(0, z), get_index(_num_cells - 1, z) };  // verticall line
            *buffer++ = uvec2 { get_index(x, 0), get_index(x, _num_cells - 1) };  // horizontal line
        }

        *buffer++ = uvec2 { get_index(_num_cells - 1, 0), get_index(_num_cells - 1, _num_cells - 1) };
        *buffer++ = uvec2 { get_index(0, _num_cells - 1), get_index(_num_cells - 1, _num_cells - 1) };
    }
    _ibo->unmap();
}

void Grid::create_vao() {
    _vao->bind();
    _vao->setAttribute<vec3>(locations::position, sizeof(vec3), 0 /* no offset */);

    _ibo->bind();
    _vbo->bind();
}

void Grid::translate(const vec3 &v) {
    _translation += v;
}

void Grid::render(const mat4 &PV) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1);
    // TODO(bkuolt): adjust OpenGL line rendering settings

    constexpr vec3 white { 1.0f, 1.0f, 1.0f };
    _program->bind();

    QMatrix4x4 matrix(glm::value_ptr(PV * glm::translate(_translation))); 
    _program->setUniformValue(locations::MVP, matrix.transposed());
    _program->setUniformValue(locations::color, white.x, white.y, white.z );

    _vao->bind();
    _ibo->bind();
    _vbo->bind();
    _vao->draw(GL_LINES, _ibo->size() / 4);
}

}  // namespace bgl
