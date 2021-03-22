/**
 * @file viewport.hpp
 * @brief A simple OpenGL Qt Viewport
 */
#ifndef BGL_VIEWPORT_HPP_
#define BGL_VIEWPORT_HPP_

#include <QOpenGLWidget>

namespace bgl {

class Viewport : public QOpenGLWidget {
 public:
	explicit Viewport(QWidget *parent);
	// TODO(bkuolt): not movable, not copyable, destructor

 protected:
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

 private:
	virtual void onDraw(float delta);
};

}  // namespace bgl

#endif  // BGL_VIEWPORT_HPP_
