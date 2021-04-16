#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

#include <string>
#include <iostream>
#include <QMainWindow>  // NOLINT

#include "../gfx/gl.hpp"    // TODO(bkuolt): fix this
#include "viewport.hpp"

namespace bgl {
namespace gui {

/**
 * @brief A simple non-copyable, but moveable Window class
 */
class Window : public QMainWindow {
 public:
    explicit Window(const std::string &title);
    Window(Window &&rhs) = default;
    Window& operator=(Window &&rhs) = default;

    Window(const Window &rhs) = delete;
    Window& operator=(const Window &rhs) = delete;

    virtual ~Window() noexcept = default;

    uvec2 getSize() const noexcept;
    void setViewport(Viewport *viewport);
};

}  // namespace gui
}  // namespace bgl

#endif  // GUI_WINDOW_HPP_
