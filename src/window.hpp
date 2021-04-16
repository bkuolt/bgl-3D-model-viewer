/**
 * @file window.hpp
 * @brief 
 */
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <string>

#include "gui/window.hpp"  // bgl::Window, bgl::Viewport
#include "gui/viewport.hpp"


namespace bgl {

/**
 * @brief 
 */
class GLViewport final : public gui::Viewport {
 public:
	explicit GLViewport(QWidget *parent);

	GLViewport(const GLViewport &) = delete;
	GLViewport &operator=(const GLViewport &) = delete;

	GLViewport(GLViewport &&) = default;
	GLViewport &operator=(GLViewport &&) = default;

	virtual ~GLViewport() = default;

	/**
	 * @brief Draws the 3D Scene.
	 */
	void draw(float delta) override;
};


/**
 * @brief 
 */
class SimpleWindow final : public gui::Window {
 public:
	SimpleWindow() noexcept = default;
	explicit SimpleWindow(const std::string &title);
	SimpleWindow(SimpleWindow &&) = default;

	SimpleWindow(const SimpleWindow &) = delete;
	SimpleWindow &operator=(const SimpleWindow &) = delete;
	SimpleWindow &operator=(SimpleWindow &&) = default;

	virtual ~SimpleWindow() noexcept = default;

 protected:
 	// bool event(QEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
};

}  // namespace bgl

#endif  // WINDOW_HPP_
