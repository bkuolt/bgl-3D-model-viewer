#include "gfx/gl.hpp"
#include "gfx/grid.hpp"

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <stdexcept>

#include "gfx/gfx.hpp"

namespace bgl {

Grid::Grid(GLfloat size, std::size_t num_cells) : _cell_size{size}, _num_cells{num_cells} {
    _meshes = std::vector<Mesh>(1);
    _program = LoadProgram("./assets/shaders/wireframe.vs", "./assets/shaders/wireframe.fs");
    create_vbo();
    create_ibo();
    create_vao();
}

void Grid::create_vbo() {
    auto get_index = [&](unsigned x, unsigned z) { return (_num_cells * z) + x; };

    const float size = static_cast<float>(_num_cells) * _cell_size;
    const vec3 T{size / 2.0f, 0.0f, size / 2.0f};

    _meshes[0]._vbo.bind();
    _meshes[0]._vbo.allocate(sizeof(vec3) * _num_cells * _num_cells);

    auto* buffer = reinterpret_cast<vec3*>(_meshes[0]._vbo.map(QOpenGLBuffer::Access::WriteOnly));
    if (buffer == nullptr) {
        throw std::runtime_error{"could not map VBO for grid creation"};
    }

    for (std::size_t z = 0; z < _num_cells; ++z) {
        for (std::size_t x = 0; x < _num_cells; ++x) {
            buffer[get_index(static_cast<unsigned>(x), static_cast<unsigned>(z))] =
                vec3{static_cast<float>(x) * _cell_size, 0.0f, static_cast<float>(z) * _cell_size} -
                T;
        }
    }
    _meshes[0]._vbo.unmap();
    _meshes[0]._vbo.release();
}

void Grid::create_ibo() {
    auto get_index = [&](unsigned x, unsigned z) {
        return static_cast<GLuint>((_num_cells * z) + x);
    };

    const std::size_t num_triangles{2 * _num_cells * _num_cells + 4};
    _meshes[0]._ibo.bind();
    _meshes[0]._ibo.allocate(num_triangles * 3 * sizeof(GLuint));

    using uvec2 = glm::tvec2<GLuint>;
    auto* buffer = reinterpret_cast<uvec2*>(_meshes[0]._ibo.map(QOpenGLBuffer::Access::WriteOnly));
    if (buffer == nullptr) {
        throw std::runtime_error{"could not map IBO for grid creation"};
    }

    for (std::size_t z = 0; z < _num_cells - 1; ++z) {
        for (std::size_t x = 0; x < _num_cells - 1; ++x) {
            const auto ux = static_cast<unsigned>(x);
            const auto uz = static_cast<unsigned>(z);
            const auto num_cells_u = static_cast<unsigned>(_num_cells);
            *buffer++ = uvec2{get_index(0, uz), get_index(num_cells_u - 1, uz)};
            *buffer++ = uvec2{get_index(ux, 0), get_index(ux, num_cells_u - 1)};
        }

        const auto uz = static_cast<unsigned>(z);
        const auto num_cells_u = static_cast<unsigned>(_num_cells);
        *buffer++ =
            uvec2{get_index(num_cells_u - 1, 0), get_index(num_cells_u - 1, num_cells_u - 1)};
        *buffer++ =
            uvec2{get_index(0, num_cells_u - 1), get_index(num_cells_u - 1, num_cells_u - 1)};
    }
    _meshes[0]._ibo.unmap();
    _meshes[0]._ibo.release();
}

void Grid::create_vao() {
    _program->bind();
    _meshes[0]._vao.bind();
    _meshes[0]._vbo.bind();
    _meshes[0]._ibo.bind();

    set_va_attribute(_program->attributeLocation("position"), 3, GL_FLOAT, 0, 0);

    _meshes[0]._vao.release();
    _meshes[0]._vbo.release();
    _meshes[0]._ibo.release();
    _program->release();
}

void Grid::translate(const vec3& v) {
    _translation += v;
}

void Grid::render(const mat4& PV) {
    if (!_program) {
        return;
    }

    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_CULL_FACE);
    glLineWidth(3.0f);

    constexpr vec3 white{1.0f, 1.0f, 1.0f};
    const QMatrix4x4 matrix(glm::value_ptr(PV * glm::translate(_translation)));

    _program->bind();
    _program->setUniformValue("MVP", matrix.transposed());
    _program->setUniformValue("color", white.x, white.y, white.z);
    _meshes[0].render(GL_LINES);
    _program->release();
}

}  // namespace bgl
