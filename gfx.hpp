// Copyright 2020 Bastian Kuolt
#ifndef GFX_HPP_
#define GFX_HPP_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <memory>
#include <filesystem>
#include <glm/glm.hpp>
#include <iomanip>
#include <iostream>

using vec2 = glm::vec2;
using vec3 = glm::vec3;

inline std::ostream& operator << (std::ostream &os, const glm::vec2 &vector) {
    os << "(" << std::fixed << std::setprecision(2) << vector.x
       << " | "
       << std::fixed << std::setprecision(2) << vector.y << ")";
    return os;
}

using shared_window = std::shared_ptr<SDL_Window>;
using shared_context = std::shared_ptr<SDL_GLContext>;
using shared_vbo = std::shared_ptr<GLuint>;
using shared_ibo = std::shared_ptr<GLuint>;
using shared_vao = std::shared_ptr<GLuint>;
using shrader_program = std::shared_ptr<GLuint>;

#ifdef __linux
namespace console_colors {
    constexpr char blue[] = "\x1B[34m";
    constexpr char red[] = "\x1B[31m";
    constexpr char white[] = "\x1B[37m";
    constexpr char magenta[] = "\x1B[35m";
    constexpr char yellow[] = "\x1B[33m";
    constexpr char green[] =  "\x1B[32m";
}
#endif  // namespace __linux

struct model {
    shared_vbo vbo;
    shared_ibo ibo;
    shared_vao vao;
    shrader_program program;
    GLsizei vertex_count;
};
using shared_model = std::shared_ptr<model>;

shared_window create_fullscreen_window();
shared_context create_GL_context(const shared_window &window);

shared_model load_model(const std::filesystem::path &path);
void render_model(const shared_model &model);

#endif  // GFX_HPP_
