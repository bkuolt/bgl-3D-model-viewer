/**
 * @file viewport.hpp
 * @brief A simple OpenGL Qt Viewport
 */
#ifndef BGL_VIEWPORT_HPP_
#define BGL_VIEWPORT_HPP_

#include <QOpenGLWidget>


namespace bgl {
namespace gui {

class Viewport : public QOpenGLWidget {
 public:
	explicit Viewport(QWidget *parent);
	Viewport(Viewport &&) = default;
	Viewport &operator=(Viewport &&) = default;

	Viewport(const Viewport &) = delete;
	Viewport &operator=(const Viewport &) = delete;

	virtual ~Viewport() noexcept = default;

	/**
	 * @brief Renders into this viewport.
	 */
	virtual void draw(float delta = 0);

 protected:
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;
};


}  // namespace gui
}  // namespace bgl

#endif  // BGL_VIEWPORT_HPP_
