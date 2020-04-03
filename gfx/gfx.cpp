// Copyright 2020 Bastian Kuolt
#include "../App.hpp"
#include "gfx.hpp"
#include "shader.hpp"


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include <algorithm>  // std::for_each()
#include <cmath>
#include <iostream>
#include <filesystem>
#include <sstream>    // std::ostringstream
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace bgl {

/* --------------------------- Grid --------------------------- */

enum locations { MVP = 0 , color, position };

grid::grid(GLfloat size, std::size_t num_cells)
    : _cell_size { size },
      _num_cells { num_cells },
      _vbo { std::make_shared<VBO>() },
      _ibo { std::make_shared<IBO>() },
      _vao { std::make_shared<VAO>(_vbo, _ibo) },
      _program(LoadProgram("./shaders/wireframe.vs", "./shaders/wireframe.fs")) {
    create_vbo();
    create_ibo();
    create_vao();
}

void grid::create_vbo() {
    auto get_index = [&](int x, int z) -> GLuint { return (_num_cells * z) + x; };

    const float size = _num_cells * _cell_size;
    const vec3 T { size / 2.0f, 0.0f, size / 2.0f };

    _vbo->resize(_num_cells * _num_cells);
    vec3 *buffer = _vbo->map();
    for (auto z = 0u; z < _num_cells; ++z) {
        for (auto x = 0u; x < _num_cells; ++x) {
            buffer[get_index(x, z)] = vec3 { x * _cell_size, 0.0, z * _cell_size } - T;
        }
    }
    _vbo->unmap();
}

void grid::create_ibo() {
    auto get_index = [&](int x, int z) { return (_num_cells * z) + x; };

    const size_t num_triangles { 2 * _num_cells * _num_cells + 4 };
    _ibo->resize(num_triangles * 3 /* vertices */);

    using uvec2 = glm::tvec2<GLuint>;
    uvec2 *buffer = reinterpret_cast<uvec2*>(_ibo->map());

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

void grid::create_vao() {
    _vao->bind();
    SetAttribute<vec3>(_vao, locations::position, sizeof(vec3), 0 /* no offset */);
    _vao->unbind();
}

void grid::render(const mat4 &PV) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1);
    // TODO(bkuolt): adjust OpenGL line rendering settings

    constexpr vec3 white { 1.0f, 1.0f, 1.0f };
    _program->use();
    _program->setUniform(locations::MVP, PV * glm::translate(_translation));
    _program->setUniform(locations::color, white);
    _vao->draw(GL_LINES);
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#if 0
void TakeScreenshot() {
    // GetColorBuffer()
    // The texture we're going to render to
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1280, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    // TODO(bkuolt)get tmp filename
    auto path = "";
    save_texture(texture, path);
    

}

#endif  // 0


}  // namespace bgl
