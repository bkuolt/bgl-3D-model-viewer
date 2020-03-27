// Copyright 2020 Bastian Kuolt
#include "gfx.hpp"
#include "shader.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>

#include <algorithm>  // std::for_each
#include <iomanip>    // std::setprecision, std::fixed
#include <iostream>

#include <sstream>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <cmath>

#include "../App.hpp"

#include <glm/gtc/matrix_transform.hpp>  // glm::lookAt(), glm::ortho()
namespace bgl {

std::ostream& operator<<(std::ostream &os, const vec2 &vector) {
    os << "(" << std::fixed << std::setprecision(2) << vector.x
       << " | "
       << std::fixed << std::setprecision(2) << vector.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream &os, const vec3 &vector) {
    os << "(" << std::fixed << std::setprecision(2) << vector.x
       << " | "
       << std::fixed << std::setprecision(2) << vector.y
       << std::fixed << std::setprecision(2) << vector.z << ")";
    return os;
}
namespace {

void initialize_SDL() {
    static bool initialized = false;

    if (!initialized) {
        const auto result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        if (result == -1) {
            throw std::runtime_error { "could not initialize SDL2" };
        }
        std::atexit(SDL_Quit);
        initialized = true;
    }
}

double calculate_aspect_ratio() {
    int width, height;
    SDL_GetWindowSize(App.window.get(), &width, &height);
    if (height == 0.0) {
        throw std::runtime_error { "invalid aspect ratio" };
    }
    return static_cast<double>(width) / height;
}

}  // namespace

std::shared_ptr<SDL_Window> createFullScreenWindow() {
    initialize_SDL();
    SDL_Window * const window = SDL_CreateWindow("BGL Tech Demo",
                                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                                 1280, 720,
                                                 SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) {
        throw std::runtime_error{ SDL_GetError() };
    }

    const auto Deleter = [] (SDL_Window *window) { SDL_DestroyWindow(window); };
    return std::shared_ptr<SDL_Window>(window, Deleter);
}

SharedContext createGLContext(const SharedWindow &window) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    auto context = SDL_GL_CreateContext(window.get());
    if (context == nullptr) {
        throw std::runtime_error { "could not create OpenGL context" };
    }

    const auto error = glewInit();
    if (GLEW_OK != error) {
        throw std::runtime_error { reinterpret_cast<const char*>(glewGetErrorString(error)) };
    }

    std::cout << "GL: "<< glGetString(GL_VERSION)
              << " GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    const auto Deleter = [] (SDL_GLContext *context) { SDL_GL_DeleteContext(context); };
    return std::shared_ptr<SDL_GLContext>(new SDL_GLContext { context }, Deleter);
}

/* -------------------------- Camera -------------------------- */

Camera::Camera(const vec3 &position, const vec3 &center)
    : _position { position }, _center { center }
{}

void Camera::setPosition(const vec3 &position) noexcept {
    _position = position;
}

void Camera::setZoom(double factor) {
    if (factor <= 0.0) {
        throw std::invalid_argument { "invalid zoom factor" };
    }
    _zoom = factor;
}

void Camera::setViewCenter(const vec3 &center) noexcept {
    _center = center;
}

const vec3& Camera::getPosition() const noexcept {
    return _position;
}

const vec3& Camera::getViewCenter() const noexcept {
    return _center;
}

double Camera::getZoom() const noexcept {
    return _zoom;
}

mat4 Camera::getMatrix() const noexcept {
    // calculates the projection matrix @p P
    const double ratio = calculate_aspect_ratio();
    const mat4 P = glm::frustum(-(ratio / 2) * _zoom, (ratio / 2) *  _zoom,
                                 -_zoom,  _zoom,
                                 1.0, 10.0);

    // calculates the view matrix @p V
    const mat4 V = glm::lookAt(_position, _center, { 0.0, 1.0, 0.0 });

    return P * V;
}

void Camera::rotate(const vec2 degrees) noexcept {
    const vec2 currentAngle { glm::radians(glm::atan(_position.x)) };
    const double radius = glm::length(_position - _center);
    const double angle = glm::radians(currentAngle.x + degrees.x);  // TODO(bkuolt): incorporate y-axis
    setPosition({ radius * glm::cos(angle), 0.0f, radius * sin(angle) });
}

/* --------------------------- Grid --------------------------- */

namespace {

constexpr size_t roundCellSize(size_t size) noexcept {
    return static_cast<size_t>((size / 2) * 2);
}

}  // namespace

enum locations { MVP = 0 , color, position };

grid::grid(size_t size)
    : _size { roundCellSize(size) },
      _vbo { std::make_shared<VBO>() },
      _ibo { std::make_shared<IBO>() },
      _vao { std::make_shared<VAO>(_vbo, _ibo) },
      _program(LoadProgram("./shaders/grid.vs", "./shaders/grid.fs")) {
    create_vbo();
    create_ibo();
    create_vao();
}

void grid::create_vbo() {
    auto get_index = [&](int x, int z) { return (_size * z) + x; };

    _vbo->resize(_size * _size);
    auto buffer = _vbo->map();
    for (auto z = 0u; z < _size; ++z) {
        for (auto x = 0u; x < _size; ++x) {
            buffer[get_index(x, z)] = vec3 { x, -1.0, z } - vec3(_size/2, 0, _size/2);
        }
    }
    _vbo->unmap();
}

void grid::create_ibo() {
    auto get_index = [&](int x, int z) { return (_size * z) + x; };

    const size_t num_triangles { 2 * _size * _size };
    _ibo->resize(num_triangles * 3 /* vertices */);
    GLuint *buffer = _ibo->map();
    for (auto z = 0u; z < _size - 1; ++z) {
        for (auto x = 0u; x < _size - 1; ++x) {
            buffer[0] = get_index(x, z);
            buffer[1] = get_index(x, z + 1);
            buffer[2] = get_index(x + 1, z + 1);
            buffer += 3;

            buffer[0] = get_index(x, z);
            buffer[1] = get_index(x + 1, z + 1);
            buffer[2] = get_index(x + 1, z);
            buffer += 3;
        }
    }
    _ibo->unmap();
}

void grid::create_vao() {
    _vao->bind();
    SetAttribute<vec3>(_vao, locations::position, sizeof(vec3), 0 /* no offset */);
    _vao->unbind();
}

void grid::render(const mat4 &MVP) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1);
    // TODO(bkuolt): adjust OpenGL line rendering settings

    constexpr vec3 white { 1.0f, 1.0f, 1.0f };
    _program->use();
    _program->setUniform(locations::MVP, MVP);
    _program->setUniform(locations::color, white);
    _vao->draw(GL_TRIANGLES);
}

}  // namespace bgl
