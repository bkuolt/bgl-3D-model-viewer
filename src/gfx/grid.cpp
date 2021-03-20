// Copyright 2021 Bastian Kuolt
#include "grid.hpp"
#include "gfx.hpp"

#include <QMatrix4x4>


namespace bgl {

enum locations { MVP = 0 , color, position };

Grid::Grid(GLfloat size, std::size_t num_cells)
    : _cell_size { size },
      _num_cells { num_cells } {

    _meshes = std::vector<Mesh>(1);
    _program = LoadProgram("./assets/shaders/wireframe.vs", "./assets/shaders/wireframe.fs");
    create_vbo();
    create_ibo();
    create_vao();
}

void Grid::create_vbo() {
    auto get_index = [&](int x, int z) -> GLuint { return (_num_cells * z) + x; };

    const float size = _num_cells * _cell_size;
    const vec3 T { size / 2.0f, 0.0f, size / 2.0f };

    _meshes[0]._vbo.bind();
    _meshes[0]._vbo.allocate(sizeof(vec3) * _num_cells * _num_cells);

    vec3 *buffer = reinterpret_cast<vec3*>(_meshes[0]._vbo.map(QOpenGLBuffer::WriteOnly));
    if (buffer == 0) {
        throw std::runtime_error { "!!!" };
    }

    for (auto z = 0u; z < _num_cells; ++z) {
        for (auto x = 0u; x < _num_cells; ++x) {
            buffer[get_index(x, z)] = vec3 { x * _cell_size, 0.0, z * _cell_size } - T;
        }
    }
    _meshes[0]._vbo.unmap();
}

void Grid::create_ibo() {
    auto get_index = [&](int x, int z) { return (_num_cells * z) + x; };

    const size_t num_triangles { 2 * _num_cells * _num_cells + 4 };
    _meshes[0]._ibo.bind();
    _meshes[0]._ibo.allocate(num_triangles * 3 * sizeof(GLuint) /* vertices */);

    using uvec2 = glm::tvec2<GLuint>;
    uvec2 *buffer = reinterpret_cast<uvec2*>(_meshes[0]._ibo.map(QOpenGLBuffer::WriteOnly));
    if (buffer == 0) {
        throw std::runtime_error { "!!!" };
    }

    for (auto z = 0u; z < _num_cells - 1; ++z) {
        for (auto x = 0u; x < _num_cells - 1; ++x) {
            *buffer++ = uvec2 { get_index(0, z), get_index(_num_cells - 1, z) };  // verticall line
            *buffer++ = uvec2 { get_index(x, 0), get_index(x, _num_cells - 1) };  // horizontal line
        }

        *buffer++ = uvec2 { get_index(_num_cells - 1, 0), get_index(_num_cells - 1, _num_cells - 1) };
        *buffer++ = uvec2 { get_index(0, _num_cells - 1), get_index(_num_cells - 1, _num_cells - 1) };
    }
    _meshes[0]._ibo.unmap();
}

void Grid::create_vao() {
    _meshes[0]._vao.bind();
    _meshes[0]._vbo.bind();
    _meshes[0]._ibo.bind();

    _program->bind();
    set_va_attribute(_program->attributeLocation("position"), 3, GL_FLOAT, 0, 0);
}

void Grid::translate(const vec3 &v) {
    _translation += v;
}

void Grid::render(const mat4 &PV) {
    std::cout << "Grid::render()" << std::endl;
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_LINE_SMOOTH);

    glDisable(GL_CULL_FACE);
    glLineWidth(3);
    // TODO(bkuolt): adjust OpenGL line rendering settings

    constexpr vec3 white { 1.0f, 1.0f, 1.0f };
    QMatrix4x4 matrix(glm::value_ptr(PV * glm::translate(_translation))); 

    _program->bind();
    _program->setUniformValue(_program->attributeLocation("MVP"), matrix.transposed());
    _program->setUniformValue(_program->attributeLocation("color"), white.x, white.y, white.z );
    _meshes[0].render(GL_LINES);
    _program->release();
}

}  // namespace bgl
