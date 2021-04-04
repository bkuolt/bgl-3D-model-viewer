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
	Viewport(Viewport&&) = default;
	Viewport& operator=(Viewport&&) = default;

	Viewport(const Viewport&) = delete;
	Viewport& operator=(const Viewport&) = delete;

	virtual ~Viewport() noexcept = default;

 protected:
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

 private:
	 virtual void on_render(float delta);
};

}  // namespace bgl

#endif  // BGL_VIEWPORT_HPP_
