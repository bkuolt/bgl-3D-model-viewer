#pragma once

/**
 * @file window.hpp
 * @brief Application specific main window and OpenGL viewport implementations.
 */

#include "gfx/gl.hpp"

#include <QKeyEvent>
#include <QWheelEvent>
#include <string>

#include "gui/gui.hpp"

namespace bgl {

class GLViewport final : public Viewport {
    Q_OBJECT

  public:
    explicit GLViewport(QWidget* parent = nullptr);
    ~GLViewport() override = default;

    GLViewport(const GLViewport&) = delete;
    GLViewport& operator=(const GLViewport&) = delete;

    GLViewport(GLViewport&&) noexcept = default;
    GLViewport& operator=(GLViewport&&) noexcept = default;

    void initializeGL() override;
    void on_render(float delta) override;
};

class SimpleWindow final : public Window {
    Q_OBJECT

  public:
    SimpleWindow() noexcept = default;
    explicit SimpleWindow(const std::string& title);
    ~SimpleWindow() noexcept override = default;

    SimpleWindow(SimpleWindow&&) noexcept = default;
    SimpleWindow& operator=(SimpleWindow&&) noexcept = default;

    SimpleWindow(const SimpleWindow&) = delete;
    SimpleWindow& operator=(const SimpleWindow&) = delete;

    bool event(QEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    GLViewport _viewport;

  private:
    bool keyEvent(QKeyEvent* event);
};

}  // namespace bgl
