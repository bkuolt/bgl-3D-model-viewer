#pragma once

/**
 * @file window.hpp
 * @brief Application specific main window and OpenGL viewport implementations.
 */

#include <QKeyEvent>
#include <QWheelEvent>
#include <memory>
#include <string>

#include "gfx/box.hpp"
#include "gfx/camera.hpp"
#include "gfx/gl.hpp"
#include "gfx/grid.hpp"
#include "gfx/model.hpp"
#include "gui/gui.hpp"

namespace bgl {

class GLViewport final : public Viewport {
    Q_OBJECT

  public:
    explicit GLViewport(QWidget* parent = nullptr);
    ~GLViewport() override;

    GLViewport(const GLViewport&) = delete;
    GLViewport& operator=(const GLViewport&) = delete;

    GLViewport(GLViewport&&) noexcept = default;
    GLViewport& operator=(GLViewport&&) noexcept = default;

    void initializeGL() override;
    void on_render(float delta) override;
    void cleanup();

    ArcBall& camera() noexcept {
        return _camera;
    }

  private:
    std::shared_ptr<Model> _model;
    std::shared_ptr<Grid> _grid;
    ArcBall _camera;
    std::shared_ptr<Box> _box;
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
