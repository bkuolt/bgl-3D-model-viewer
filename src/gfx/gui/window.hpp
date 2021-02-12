// Copyright 2020 Bastian Kuolt
#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

//////////////////////////////////////
//////////////////////////////////////
#define USE_SDL2  // TODO(bkuolt): make configurable with compile flag
// USE_QT
//////////////////////////////////////
//////////////////////////////////////

#include "../gl/gl.hpp"  // TODO()

#include <memory>
#include <string>

#if defined(USE_SDL2)
#include <SDL2/SDL_video.h>  // SDL_GLContext, SDL_Window
#include <SDL2/SDL_events.h>
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
    explicit Window(const std::string &title, bool windowed = true);
    explicit Window(Window &&);
    Window(const Window&) = delete;
    virtual ~Window() noexcept;

    Window& operator=(Window &&rhs);
    Window& operator=(const Window&) = delete;

#if defined(USE_QT)
	Window();
	bool event(QEvent *event) override;
#endif  // USE_QT

    int exec();
    void close() noexcept;

    void show() noexcept;
    void hide() noexcept;
    uvec2 getSize() const noexcept;

#if defined(USE_SDL2)
    SDL_GLContext getOpenGLContext() noexcept;
    SDL_Window *getHandle() noexcept;
#endif  // USE_SDL2

    void render();

#if defined(USE_SDL2)
    virtual void on_key(const SDL_KeyboardEvent &event) { /* intenionally does nothing */ }
    virtual void on_render(float delta) noexcept { /* intenionally does nothing */ }
#endif  // USE_SDL2

 private:
    void swap(Window &rhs) noexcept;

#if defined(USE_SDL2)
    SDL_Window *_window { nullptr };
    SDL_GLContext _context { nullptr };
#elif  defined(USE_QT)
	GLViewport _viewport;
	// TODO(bkuolt): handle events
#endif

    bool _run { false };
};

}  // namespace bgl

#endif  // GUI_WINDOW_HPP_
