#pragma once

/**
 * @file viewport.hpp
 * @brief Qt 6 OpenGL Viewport Widget.
 */

#include <QOpenGLWidget>

#include "gfx/gl.hpp"

namespace bgl {

class Viewport : public QOpenGLWidget {
    Q_OBJECT

  public:
    explicit Viewport(QWidget* parent = nullptr);
    ~Viewport() noexcept override = default;

    Viewport(Viewport&&) noexcept = default;
    Viewport& operator=(Viewport&&) noexcept = default;

    Viewport(const Viewport&) = delete;
    Viewport& operator=(const Viewport&) = delete;

  protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

  private:
    virtual void on_render(float delta);
};

}  // namespace bgl
