/**
 * @file OpenGLViewport.cpp
 * @brief A simple Qt OpenGL viewport.
 */
#ifndef BGL_GL_VIEWPORT_HPP_
#define BGL_GL_VIEWPORT_HPP_

#include <GL/glew.h>
#include <QOpenGLWidget>

#include <memory>

#include "Scene.hpp"


namespace bgl {
namespace gui {

class OpenGLViewport : public QOpenGLWidget {
	Q_OBJECT
 public:
    explicit OpenGLViewport(QWidget *parent = nullptr);
	virtual ~OpenGLViewport() noexcept = default;

	std::shared_ptr<Scene> getScene() noexcept;
	void setScene(const std::shared_ptr<Scene> &scene);

 protected:
	void paintEvent(QPaintEvent *event) override;

	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

 private:
	std::shared_ptr<Scene> _scene;
};

}  // namespace gui
}  // namespace bgl

#endif  // BGL_GLVIEWPORT_HPP_
