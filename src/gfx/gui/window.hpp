// Copyright 2020 Bastian Kuolt
#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

#include "../gl/gl.hpp"  // TODO(bkuolt): fix this

//#include <memory>
#include <string>

#include <QMainWindow>
#include "viewport.hpp"


namespace bgl {

/**
 * @brief A simple non-copyable, but moveable Window class
 */
class Window : public QMainWindow {
 public:
    explicit Window(const std::string &title);

	// TODO(bkuolt): not movable, not copyable, destructor

    Window(const Window&) = delete;
    virtual ~Window() noexcept;

    Window& operator=(Window &&rhs);
    Window& operator=(const Window&) = delete;

    uvec2 getSize() const noexcept;
    void render();
    void setViewport(Viewport *p);

 protected:
	Viewport *_viewport { nullptr };

 private:
    void swap(Window &rhs) noexcept;
};

}  // namespace bgl

#endif  // GUI_WINDOW_HPP_
