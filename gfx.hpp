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


using shared_window = std::shared_ptr<SDL_Window>;
using shared_context = std::shared_ptr<SDL_GLContext>;
using shared_vbo = std::shared_ptr<GLuint>;
using shared_ibo = std::shared_ptr<GLuint>;
using shared_vao = std::shared_ptr<GLuint>;

struct model {
    shared_vbo vbo;
    shared_ibo ibo;
    shared_vao vao;
};
using shared_model = std::shared_ptr<model>;

shared_window create_fullscreen_window();
shared_context create_GL_context(const shared_window &window);
shared_model load_model(const std::filesystem::path &path);

#endif  // GFX_HPP_
