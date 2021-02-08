// Copyright 2020 Bastian Kuolt
#include "../App.hpp"
#include "gfx.hpp"
#include "window.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>


#include <iostream>
#include <set>
#include <future>    // std::call_once
#include <utility>   // std::swap()
#include <limits>
#include <memory>
#include <string>
#include <vector>


namespace bgl {

namespace {

class frame_counter {
 public:
    bool count() noexcept {
        _delta = (SDL_GetTicks() - _timestamp_render) / 1000.0f;
        _timestamp_render = SDL_GetTicks();
        ++_num_frames;

        const bool is_new_second { SDL_GetTicks() - _timestamp_fps >= 1000 };
        if (is_new_second) {
            _timestamp_fps = SDL_GetTicks();
            _fps = _num_frames;
        }

        return is_new_second;
    }

    size_t fps() const noexcept {
        return  _fps;
    }

    double delta() const noexcept {
        return _delta;
    }

 private:
    Uint32 _timestamp_fps { 0 };
    Uint32 _timestamp_render { 0 };
    size_t _num_frames { 0 };
    size_t _fps { 0 };
    double _delta { 0.0 };
};

SDL_GLContext create_OpenGL_Context(SDL_Window *window) {
    const bool successfull {
           !SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4)
        && !SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5)
        && !SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)
        && !SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)
        && !SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)
        && !SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1)
        && !SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16)
        &&  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")
    };

    if (!successfull) {
        throw std::runtime_error { SDL_GetError() };
    }

    SDL_GLContext context { SDL_GL_CreateContext(window) };
    if (context == nullptr) {
        throw std::runtime_error { "could not create OpenGL context" };
    } else if (SDL_GL_SetSwapInterval(0) == -1) {
        throw std::runtime_error { "could not disable vsync" };
    }

    const GLenum error { glewInit() };
    if (GLEW_OK != error) {
        throw std::runtime_error { reinterpret_cast<const char*>(glewGetErrorString(error)) };
    }

    std::cout << "created rendering context: "
              << "GL: "   << glGetString(GL_VERSION) << " "
              << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    return context;
}

SDL_Window* create_window(const std::string &title, bool windowed = false) {
    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0 /* default device */, &displayMode) != 0) {
        throw std::runtime_error { SDL_GetError() };
    }

    SDL_Window *window {
        SDL_CreateWindow(title.c_str(),
                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         displayMode.w * 0.75, displayMode.h * 0.75,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | (!windowed ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)) };

    return window ? window : throw std::runtime_error{ SDL_GetError() };
}

std::once_flag SDL_initialization_flag;

void initialize_SDL() {
    const auto result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (result == -1) {
        throw std::runtime_error { "could not initialize SDL2" };
    }
    std::atexit(SDL_Quit);
}

/* ------------------- Event Handling ------------------- */
/**
 * @brief The currently managed window.
 * @note Currently there is only support for one window.
 */
Window* _current_window = nullptr;  // TODO(bkuolt): Fix this mess

/**
 * @brief Handles all events of all windows.
*/
void handle_event(const SDL_Event &event) {
    switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            on_key(event.key);
            break;
        case SDL_JOYAXISMOTION:
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            // TODO(bkuolt): add gamepad support
            break;
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            if (_current_window) _current_window->close();
            break;
    }
}

}  // anonymous namespace


Window::Window(const std::string &title, bool windowed) {
    std::call_once(SDL_initialization_flag, &initialize_SDL);
    _window = create_window(title, windowed);
    _context = create_OpenGL_Context(_window);
    _run = true;

    _current_window = this;  // TODO(bkuolt)
}

Window::Window(Window &&rhs) {
    swap(rhs);
}

Window::~Window() noexcept {
    SDL_DestroyWindow(_window);
    SDL_GL_DeleteContext(_context);

    _current_window = nullptr;  // TODO(bkuolt)
}

Window& Window::operator=(Window &&rhs) {
    swap(rhs);
    return *this;
}

void Window::swap(Window &rhs) noexcept {
    std::swap(_window, rhs._window);
    std::swap(_context, rhs._context);
}

void Window::close() noexcept {
    _run = false;
}

int Window::exec() {
    SDL_Event event;
    while (_run) {
        while (SDL_PollEvent(&event)) {
            try {
                handle_event(event);
            } catch (const std::exception &error) {
                std::cerr << error.what();
                return EXIT_FAILURE;
            }
        }
        render();
    }

    std::cout << "\r" << console_color::white << std::endl;
    SDL_DestroyWindow(getHandle());  // TODO(bkuolt): make sure that the window is destroyed before the context
    return EXIT_SUCCESS;
}

SDL_GLContext Window::getOpenGLContext() noexcept {
    return _context;
}

SDL_Window *Window::getHandle() noexcept {
    return _window;
}

void Window::show() noexcept {
    SDL_ShowWindow(_window);
}

void Window::hide() noexcept {
    SDL_HideWindow(_window);
}

uvec2 Window::getSize() const noexcept {
    int width, height;
    SDL_GetWindowSize(_window, &width, &height);
    return { width, height };
}

void Window::render() {
    static frame_counter frame_counter;
    const bool changed { frame_counter.count() };
    on_render(frame_counter.delta());
    SDL_GL_SwapWindow(_window);

    if (changed) {
        // TODO(bkuolt): add TTF font rendering support
        std::cout << "\r" << console_color::blue << frame_counter.fps() << " FPS" << std::flush;
    }
}

SharedWindow createWindow(const std::string &title, bool windowed) {
    return std::make_shared<Window>(title, windowed);
}

}  // namespace bgl
