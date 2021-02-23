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

namespace {

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

}  // anonymous namespace

Box::Box() {
    // create vbo
    _vbo->bind();
    _vbo->allocate(box_vertices.size() * sizeof(vec3));
    std::copy(box_vertices.begin(), box_vertices.end(), reinterpret_cast<vec3*>(_vbo->map(QOpenGLBuffer::WriteOnly)));
    _vbo->unmap();

    // create ibo
    _ibo->bind();
    _ibo->allocate(box_indices.size() * 2 * sizeof(GLuint));
    uvec2 *buffer = reinterpret_cast<uvec2*>(_ibo->map(QOpenGLBuffer::WriteOnly));
    std::copy(box_indices.begin(), box_indices.end(), buffer);
    _ibo->unmap();

    // create vao
    _vao->bind();
    _ibo->bind();
    _vbo->bind();
    _vao->setAttribute<vec3>(2 /*locations::position*/, 0 /* no stride */, 0 /* no offset */);

    _program = LoadProgram("./gfx/shaders/wireframe.vs", "./gfx/shaders/wireframe.fs");
}

Box::Box(const BoundingBox &boundingBox)
    : Box() {
     _boundingBox = boundingBox;
}

void Box::render(const mat4 &VP) {
    _program->bind();
    glLineWidth(3);

    mat4 M = glm::scale(_boundingBox.getSize());
    QMatrix4x4 matrix(glm::value_ptr(VP * M));
    _program->setUniformValue("MVP", matrix.transposed());

    const vec3 color { 1.0, 0.0, 0.0 }; /* red */
    _program->setUniformValue("color", color.x, color.y, color.z);

    _vao->bind();
    _ibo->bind();
    _vbo->bind();
    _vao->draw(GL_LINES, box_indices.size() * 2);

    _program->release();
    _vao->release();
    _ibo->release();
    _vbo->release();
}

}  // namespace bgl
