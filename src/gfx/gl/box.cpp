// Copyright 2021 Bastian Kuolt
#include "box.hpp"

#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <filesystem>

#include <QMatrix4x4>


namespace bgl {

std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::filesystem::path &vs, const std::filesystem::path &fs);

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


using uvec2 = glm::tvec2<GLuint>;
constexpr std::array<vec3, 8> box_vertices {{
    { -0.5, -0.5, 0.5 }, { -0.5,  0.5, 0.5 },    // front, left handside
    {  0.5,  0.5, 0.5 }, {  0.5, -0.5, 0.5 },    // front, right handside
    { -0.5, -0.5, -0.5 }, { -0.5,  0.5, -0.5 },  // back, left handside
    {  0.5,  0.5, -0.5 }, {  0.5, -0.5, -0.5 }   // back, right handside
}};

/* 
front: back:
1--2   5--6 
|  |   |  |
0--3   4--7
*/
static constexpr std::array<uvec2, 12> box_indices {{
    { 0, 3 }, { 1, 2 }, { 0, 1 }, { 2, 3 },  // frontside
    { 4, 7 }, { 5, 6 }, { 4, 5 }, { 6, 7 },  // backside
    {0, 4}, {1, 5}, {3, 7}, {2, 6}  // left and right sides
}};

Box::Box(const vec3 &dimensions)
    : _dimensions { dimensions },
      _vbo { std::make_shared<VertexBuffer<vec3>>() },
      _ibo { std::make_shared<IndexBuffer>() },
      _vao { std::make_shared<VertexArray<vec3>>(_vbo, _ibo) } {
    // create vbo
    _vbo->resize(box_vertices.size());
    std::copy(box_vertices.begin(), box_vertices.end(), _vbo->map());
    _vbo->unmap();

    // create ibo
    _ibo->resize(box_indices.size() * 2);
    uvec2 *buffer = reinterpret_cast<uvec2*>(_ibo->map());
    std::copy(box_indices.begin(), box_indices.end(), buffer);
    _ibo->unmap();

    // create vao
    _vao->bind();
    SetAttribute<vec3>(_vao, 2 /*locations::position*/, sizeof(vec3), 0 /* no offset */);
    _vao->unbind();

    _program = LoadProgram("./assets/shaders/wireframe.vs", "./assets/shaders/wireframe.fs");
}

Box::Box(GLfloat size)
    : Box({ size, size, size })
{}

void Box::render(const mat4 &VP) {
    _program->bind();
    glLineWidth(3);

    mat4 M = glm::scale(_dimensions);
   
    QMatrix4x4 matrix(glm::value_ptr(VP * M));
    _program->setUniformValue("MVP", matrix.transposed());

    const vec3 color { 1.0, 0.0, 0.0 }; /* red */
    _program->setUniformValue("color", color.x, color.y, color.z);

    _vao->bind();
    _vao->draw(GL_LINES);
    _vao->unbind();
}

void Box::resize(const vec3 &dimensions) {
    _dimensions = dimensions;
}

}  // namespace bgl