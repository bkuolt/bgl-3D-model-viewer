// Copyright 2021 Bastian Kuolt
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "gfx/gfx.hpp"

#include <QOpenGLFramebufferObject>
#include <string>

class QEvent;
class QKeyEvent;


namespace bgl {

class GLViewport final : public Viewport {
 public:
	explicit GLViewport(QWidget *parent);
    virtual ~GLViewport() = default;

	void on_render(float delta = 1.0) override;
	void takeScreenshot();
 private:
 	QOpenGLFramebufferObject *_fbo;
};

class SimpleWindow final : public bgl::Window {
 public:
	explicit SimpleWindow(const std::string &title);
    virtual ~SimpleWindow() noexcept = default;
	// TODO(bkuolt): not movable, not copyable, destructor

	bool event(QEvent *event) override;

 private:
	bool keyEvent(QKeyEvent *event);
    GLViewport _viewport;
};

}  // namespace bgl

#endif  // WINDOW_HPP_
