#pragma once

/**
 * @file window.hpp
 * @brief Base main application window for the GUI.
 */

#include <QMainWindow>
#include <string>

#include "gfx/gl.hpp"
#include "gfx/math.hpp"
#include "gui/viewport.hpp"

namespace bgl {

/**
 * @brief Main application window managing menu bar, status bar, and central viewport.
 */
class Window : public QMainWindow {
    Q_OBJECT

  public:
    explicit Window(const std::string& title);
    ~Window() noexcept override = default;

    Window(Window&&) noexcept = default;
    Window& operator=(Window&&) noexcept = default;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    [[nodiscard]] uvec2 getSize() const noexcept;
    void render();
    void setViewport(Viewport* p);

  protected:
    Viewport* _viewport{nullptr};
};

}  // namespace bgl
