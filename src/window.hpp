/**
 * @file window.hpp
 * @brief 
 */
#include <QKeyEvent>

#include <string>

#include "gui/gui.hpp"  // bgl::Window, bgl::Viewport


namespace bgl {

/**
 * @brief 
 */
class GLViewport final : public Viewport {
 public:
	explicit GLViewport(QWidget *parent);

	GLViewport(const GLViewport&) = delete;
	GLViewport& operator=(const GLViewport&) = delete;

	GLViewport(GLViewport&&) = default;
	GLViewport& operator=(GLViewport&&) = default;

    virtual ~GLViewport() = default;

	void on_render(float delta) override;
};

/**
 * @brief 
 */
class SimpleWindow final : public Window {
 public:
	SimpleWindow() noexcept = default;
	explicit SimpleWindow(const std::string &title);
	SimpleWindow(SimpleWindow&&) = default;

	SimpleWindow(const SimpleWindow&) = delete;
	SimpleWindow& operator=(const SimpleWindow&) = delete;
	SimpleWindow& operator=(SimpleWindow&&) = default;

	virtual ~SimpleWindow() noexcept = default;

	bool event(QEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

    GLViewport _viewport;  // TODO
 private:
	bool keyEvent(QKeyEvent *event);
};

}  // namespace bgl

// #endif  // WINDOW_HPP_
