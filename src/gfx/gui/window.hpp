// Copyright 2020 Bastian Kuolt
#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

#include "../gl/gl.hpp"  // TODO(bkuolt): fix this

#include <memory>
#include <string>

#if defined(USE_SDL2)
#include <SDL2/SDL_video.h>   // SDL_GLContext, SDL_Window
#include <SDL2/SDL_events.h>  // SDL_KeyboardEvent
#elif defined(USE_QT)
#include <QMainWindow>
#include "GLViewport.hpp"
#endif

namespace bgl {

/**
 * @brief A simple non-copyable, but moveable Window class
 */
class Window
#if defined(USE_QT)
 : public QMainWindow
#endif  // USE_QT
{
 public:
    explicit Window(const std::string &title = "", bool windowed = true);
    explicit Window(Window &&);
    Window(const Window&) = delete;
    virtual ~Window() noexcept;

    Window& operator=(Window &&rhs);
    Window& operator=(const Window&) = delete;

#if defined(USE_SDL2)
    void close() noexcept;
    void show() noexcept;
    void hide() noexcept;
#endif  // USE_SDL2

#if defined(USE_SDL2)
    int exec();
#elif defined(USE_QT)
	bool event(QEvent *event) override;
#endif  // USE_QT

    uvec2 getSize() const noexcept;
    void render();

#if defined(USE_SDL2)
    virtual void on_key(const SDL_KeyboardEvent &event) { /* intenionally does nothing */ }
    virtual void on_render(float delta) noexcept { /* intenionally does nothing */ }
#endif  // USE_SDL2

 private:
    void swap(Window &rhs) noexcept;

#if defined(USE_SDL2)
    bool _run { false };
    SDL_Window *_window { nullptr };
    SDL_GLContext _context { nullptr };
#elif  defined(USE_QT)
	GLViewport _viewport;
	// TODO(bkuolt): handle events
#endif
};

}  // namespace bgl

#endif  // GUI_WINDOW_HPP_
