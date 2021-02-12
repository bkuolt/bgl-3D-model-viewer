// Copyright 2020 Bastian Kuolt
#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

#include "../gl/gl.hpp"  // TODO(bkuolt): fix this

#include <memory>
#include <string>

#include <QMainWindow>
#include "GLViewport.hpp"


namespace bgl {

/**
 * @brief A simple non-copyable, but moveable Window class
 */
class Window : public QMainWindow {
 public:
    explicit Window(const std::string &title);

    // TODO(bkuolt): explicit Window(Window &&);
    Window(const Window&) = delete;
    virtual ~Window() noexcept;

    Window& operator=(Window &&rhs);
    Window& operator=(const Window&) = delete;

    uvec2 getSize() const noexcept;
    void render();
    void setViewport(GLViewport *p);

 protected:
	GLViewport *_viewport { nullptr };

 private:
    void swap(Window &rhs) noexcept;
	// TODO(bkuolt): handle events
};

}  // namespace bgl

#endif  // GUI_WINDOW_HPP_
