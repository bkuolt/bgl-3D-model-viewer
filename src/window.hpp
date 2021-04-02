/**
 * @file window.hpp
 * @brief 
 */
#include <string>
#include <QKeyEvent>

#include "gfx/gfx.hpp"  // bgl::Window, bgl::Viewport


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
 * 
 */
class SimpleWindow final : public bgl::Window {
 public:
	SimpleWindow() noexcept = default;
	explicit SimpleWindow(const std::string &title);

	SimpleWindow(const SimpleWindow&) = delete;
	SimpleWindow& operator=(const SimpleWindow&) = delete;

	SimpleWindow(SimpleWindow&&) = default;
	SimpleWindow& operator=(SimpleWindow&&) = default;

	bool event(QEvent *event) override;

    GLViewport _viewport;  // TODO
 private:
	bool keyEvent(QKeyEvent *event);
};

}  // namespace bgl
