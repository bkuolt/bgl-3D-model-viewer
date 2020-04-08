// Copyright 2020 Bastian Kuolt
#ifndef GFX_WINDOW_HPP_
#define GFX_WINDOW_HPP_

#include "gl/gl.hpp"

#include <memory>
#include <string>

#include <SDL2/SDL_video.h>  // SDL_GLContext, SDL_Window

namespace bgl {

/**
 * @brief A simple non-copyable, but moveable Window class
 */
class Window {
 public:
    explicit Window(const std::string &title, bool windowed);
    explicit Window(Window &&);
    Window(const Window&) = delete;
    virtual ~Window() noexcept;

    Window& operator=(Window &&rhs);
    Window& operator=(const Window&) = delete;

    void show() noexcept;
    void hide() noexcept;
    uvec2 getSize() const noexcept;

    SDL_GLContext getOpenGLContext() noexcept;
    SDL_Window *getHandle() noexcept;

    void render();

 private:
    void swap(Window &rhs) noexcept;

    SDL_Window *_window { nullptr };
    SDL_GLContext _context { nullptr };
};

using SharedWindow = std::shared_ptr<Window>;
SharedWindow createWindow(const std::string &title = "", bool windowed = false);

}  // namespace bgl

#endif  // GFX_WINDOW_HPP_
