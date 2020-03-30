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
#include <set>
#include <sstream>    // std::ostringstream
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace bgl {

std::set<Window*> windows;

namespace {

SDL_GLContext create_OpenGL_Context(SDL_Window *window) {
    const bool successfull {
           !SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4)
        && !SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5)
        && !SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)
        && !SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)
        && !SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)
    };

    if (!successfull) {
        throw std::runtime_error { SDL_GetError() };
    }

    SDL_GLContext context { SDL_GL_CreateContext(window) };
    if (context == nullptr) {
        throw std::runtime_error { "could not create OpenGL context" };
    }
    if (SDL_GL_SetSwapInterval(0) == -1) {
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

}  // namespace

Window::Window(const std::string &title, bool windowed) {
    std::call_once(SDL_initialization_flag, &initialize_SDL);
    _window = create_window(title, windowed);
    _context = create_OpenGL_Context(_window);
    windows.emplace(this);
}

Window::Window(Window &&rhs) {
    swap(rhs);
}

Window::~Window() noexcept {
    SDL_DestroyWindow(_window);
    SDL_GL_DeleteContext(_context);
    windows.erase(this);
}

Window& Window::operator=(Window &&rhs) {
    swap(rhs);
    return *this;
}

void Window::swap(Window &rhs) noexcept {
    std::swap(_window, rhs._window);
    std::swap(_context, rhs._context);
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
    using namespace std::chrono_literals;

    static Uint32 timestamp_render = SDL_GetTicks();
    static Uint32 timestamp_fps = SDL_GetTicks();
    static size_t fps = 0;


    const Uint32 duration = SDL_GetTicks() - timestamp_render;
    const float delta = duration / 1000.0f;
    timestamp_render = SDL_GetTicks();
    on_render(::App.window, delta);
    ++fps;

    // track fps
    if (SDL_GetTicks() - timestamp_fps >= 1000) {
        timestamp_fps = timestamp_render;
        std::cout << console_color::blue << "\r" << fps << " fps" << std::flush;
        fps = 0;
    }
}

SharedWindow createWindow(const std::string &title, bool windowed) {
    return std::make_shared<Window>(title, windowed);
}

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
    const vec3 T { size / 2.0f, 1.0f, size / 2.0f };

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

void grid::render(const mat4 &MVP) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1);
    // TODO(bkuolt): adjust OpenGL line rendering settings

    constexpr vec3 white { 1.0f, 1.0f, 1.0f };
    _program->use();
    _program->setUniform(locations::MVP, MVP);
    _program->setUniform(locations::color, white);
    _vao->draw(GL_LINES);
}

/* -------------------------------- Font ------------------------------ */

namespace {

std::once_flag is_TTF_initialized;

void initialize_TTF() {
    if (TTF_Init() == -1) {
        throw std::runtime_error { "could not initialize SDL TTF" };
    }
    std::atexit(TTF_Quit);
}

SDL_Renderer* getRenderer() {
    SDL_Renderer *renderer { SDL_GetRenderer(App.window->getHandle()) };
    if (renderer == nullptr) {
        throw std::runtime_error { "could not get SDL Renderer" };
    }
    return renderer;
}

}  // namespace

Font::Font(const std::filesystem::path &path, std::size_t size)
    : _size { size } {
    std::call_once(is_TTF_initialized, &initialize_TTF);
    _handle = TTF_OpenFont(path.string().c_str(), _size);
    if (_handle == nullptr) {
        throw std::runtime_error { "could not load font" };
    }
}

Font::~Font() noexcept {
    TTF_CloseFont(_handle);
}

SharedText Font::createText(const std::string &text) {
    return std::make_shared<Text>(shared_from_this(), text);
}

/* --------------------------------- Text --------------------------------- */

Text::Text(const SharedFont &font, const std::string &text)
    : _font { font } {
//    setText(text);
}

Text::~Text() noexcept {
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
    }
}

SDL_Texture* getSDLTexture(SDL_Surface *surface) {
    SDL_Texture *texture { SDL_CreateTextureFromSurface(getRenderer(), surface) };
    if (texture == nullptr) {
        throw std::runtime_error { SDL_GetError() };
    }
    return texture;
}

void Text::setText(const std::string &text) {
    const SDL_Color color { 255, 255, 255 };

    if (_texture) {  // delete previous texture
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(_font->_handle, text.c_str(), color);
    if (surface == nullptr) {
        throw std::runtime_error { TTF_GetError() };
    }

    _texture = getSDLTexture(surface);
    SDL_FreeSurface(surface);
    if (_texture == nullptr) {
        throw std::runtime_error { TTF_GetError() };
    }

    // recalculate bounding box
    _text = text;
    if (TTF_SizeText(_font->_handle, _text.c_str(), &_dimensions.x, &_dimensions.y) == -1) {
        throw std::runtime_error { TTF_GetError() };
    }
}

void Text::render() {
    SDL_Renderer *renderer { getRenderer() };
    const SDL_Rect rectangle { 0, 0, _dimensions.x, _dimensions.y };
    if (SDL_RenderCopy(renderer, _texture, nullptr, &rectangle) == -1) {
        throw std::runtime_error { "could not render font" };
    }
}

}  // namespace bgl
