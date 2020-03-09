// Copyright 2020 Bastian Kuolt
#ifndef GFX_HPP_
#define GFX_HPP_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <ostream>
#include <memory>
#include <filesystem>

#include <glm/glm.hpp>

/* ---------------------------- Basic Types ---------------------------- */

using vec2 = glm::vec2;
using vec3 = glm::vec3;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoords;
};

using shared_window = std::shared_ptr<SDL_Window>;      // TODO(bkuolt): rename
using shared_context = std::shared_ptr<SDL_GLContext>;  // TODO(bkuolt): rename

/* --------------------------------------------------------------------- */

shared_window create_fullscreen_window();
shared_context create_GL_context(const shared_window &window);

/* --------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream &os, const vec2 &vector);

#ifdef __linux
namespace console_colors {  // TODO(bkuolt): rename to "console color"
    constexpr char blue[] = "\x1B[34m";
    constexpr char red[] = "\x1B[31m";
    constexpr char white[] = "\x1B[37m";
    constexpr char magenta[] = "\x1B[35m";
    constexpr char yellow[] = "\x1B[33m";
    constexpr char green[] =  "\x1B[32m";
}
#endif  // namespace __linux

/* --------------------------------------------------------------------- */

using shared_vbo = std::shared_ptr<GLuint>;      // TODO(bkuolt): rename
using shared_ibo = std::shared_ptr<GLuint>;      // TODO(bkuolt): rename
using shared_vao = std::shared_ptr<GLuint>;      // TODO(bkuolt): rename
using shader_program = std::shared_ptr<GLuint>;  // TODO(bkuolt): rename

using SharedShader = std::shared_ptr<GLuint>;
using SharedProgram = std::shared_ptr<GLuint>;

struct Model {
    const shared_vbo vbo;
    const shared_ibo ibo;
    const shared_vao vao;
    const GLsizei vertex_count;

    const std::shared_ptr<GLuint> texture;
    const shader_program program;
};

using SharedModel = std::shared_ptr<Model>;
SharedModel LoadModel(const std::filesystem::path &path);

void render_model(const SharedModel &model);

#endif  // GFX_HPP_
