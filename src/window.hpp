#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "gfx/gfx.hpp"
#include "gfx/gui/viewport.hpp"
#include "gfx/gui/window.hpp"

#include <string>

class QKeyEvent;


namespace bgl {

class GLViewport final : public Viewport {
 public:
	explicit GLViewport(QWidget *parent);
	// TODO(bkuolt): not movable, not copyable, destructor
    virtual ~GLViewport() = default;

	void initializeGL() override { /* TODO */ };

	void onDraw(float delta) override;
};


class SimpleWindow final : public bgl::Window {
 public:
	explicit SimpleWindow(const std::string &title);
	SimpleWindow(SimpleWindow&&) noexcept = default;
	SimpleWindow& operator=(SimpleWindow&&) noexcept = default;

	SimpleWindow(const SimpleWindow&) noexcept = delete;
	SimpleWindow& operator=(const SimpleWindow&) noexcept = delete;

    virtual ~SimpleWindow() noexcept = default;

	bool event(QEvent *event) override;

 private:
    GLViewport _viewport;

	bool keyEvent(QKeyEvent *event);
};

}  // namespace bgl

#endif  // WINDOW_HPP_
