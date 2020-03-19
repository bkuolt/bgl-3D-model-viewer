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


#include "../App.hpp"

#include <glm/gtc/matrix_transform.hpp>  // glm::lookAt(), glm::ortho()
namespace bgl {

std::ostream& operator<<(std::ostream &os, const vec2 &vector) {
    os << "(" << std::fixed << std::setprecision(2) << vector.x
       << " | "
       << std::fixed << std::setprecision(2) << vector.y << ")";
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

}  // namespace

std::shared_ptr<SDL_Window> createFullScreenWindow() {
    initialize_SDL();
    SDL_Window * const window = SDL_CreateWindow("BGL Tech Demo",
                                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                                 1280, 720,
                                                 SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN /* | SDL_WINDOW_FULLSCREEN_DESKTOP */);
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


//////////////////////////////////
//////////////////////////////////


Camera::Camera(const vec3 &position, const vec3 &viewCenter) {
    // TODO(bkuolt):
}

void Camera::setPosition(const vec3 &position) noexcept {
    _position = position;
}

void Camera::setZoom(double factor) noexcept {
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
    int width, height;
    SDL_GetWindowSize(App.window.get(), &width, &height);
    const double ratio = static_cast<double>(width) / height;
    const mat4 P = glm::frustum(-ratio / 2, ratio / 2,
                                -1.0, 1.0,
                                1.0, 5.0);
    // TODO(bkuolt): incorporate zoom factor

    // calculates the view matrix @p V
    const mat4 V = glm::lookAt(_position, _center, { 0.0, 1.0, 0.0 });

    return V * P;
}

void Camera::rotate(const vec2 degrees) noexcept {
    const vec2 currentAngle {};  // TODO(bkuolt): get angle
    const double radius = 0.0;      // TODO(bkuolt): get radius
    const double angle = glm::radians(currentAngle.x + degrees.x);  // TODO(bkuolt): incorporate y-axis
    setPosition({ radius * glm::cos(angle), 0.0f, radius * sin(angle) });
}

#if 0
class grid final {
 public:
    explicit grid(size_t size);
    void render();

 private:
    void create_vbo();
    void create_ibo();
    void create_vao();

    Buffer<vec3, GL_ARRAY_BUFFER> _vbo;
    IndexBuffer _ibo;
    // TODO(bkuolt): vao
    // TODO(bkuolt): shader
    const GLsizei _size;
};

grid::grid(size_t size)
    : _size { size },
      _vbo { _size * 4 },
      _ibo { _size * 2} {
    create_vbo();
    create_ibo();
    create_vao();
    // TODO(bkuolt): load shader
}

void grid::create_vbo() {
    auto buffer = _vbo.map();
    for (auto i = 0u; i < _size; ++i) {
        // horizontally
        *buffer++ = vec3 { -_size / 2 + i, 0.0f, -_size / 2 };
        *buffer++ = vec3 { -_size / 2 + i, 0.0f,  _size / 2 };
        // vertically
        *buffer++ = vec3 { -_size / 2, 0.0f, -_size / 2 + i};
        *buffer++ = vec3 {  _size / 2, 0.0f, -_size / 2 + i};
        }
    }
    _vbo.unmap();
}

void grid::create_ibo() {
    auto buffer = _ibo.map();
    // TODO(bkuolt): calculate line 
    _ibo.unmap();
}

void grid::create_vao() {
    // TODO(bkuolt)
}

void grid::render() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    _vao.bind();
    // TODO(render)
}
#endif  // 0

}  // namespace bgl
