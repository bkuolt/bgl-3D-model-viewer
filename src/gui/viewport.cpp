/**
 * @file viewport.cpp
 * @brief A simple OpenGL Qt Viewport
 */
#include "../gfx/gl.hpp"

#include <cassert>
#include <ctime>     // std::clock()
#include <iostream>

#include <QOpenGLWidget>

#include "viewport.hpp"


#if 0
namespace {

inline clock_t SDL_GetTicks()  {  // TODO(bkuolt)
	return (std::clock() / static_cast<double>(CLOCKS_PER_SEC)) * 1000 *1000;
}

class frame_counter final {
 public:
	bool count() noexcept {
		_delta = (SDL_GetTicks() - _timestamp_render) / 1000.0f;
		_timestamp_render = SDL_GetTicks();
		++_num_frames;

		const bool is_new_second { SDL_GetTicks() - _timestamp_fps >= 1000 };
		if (is_new_second) {
			_timestamp_fps = SDL_GetTicks();
			_fps = _num_frames;
		}

		return is_new_second;
	}

	size_t fps() const noexcept {
		return  _fps;
	}

	double delta() const noexcept {
		return _delta;
	}

 private:
	uint32_t _timestamp_fps { 0 };
	uint32_t _timestamp_render { 0 };
	size_t _num_frames { 0 };
	size_t _fps { 0 };
	double _delta { 0.0 };
};

}  // anonymous namespace
#endif  // 0

namespace bgl {
namespace gui {

Viewport::Viewport(QWidget *parent)
    : QOpenGLWidget{ parent } {
	assert(parent != nullptr);
}

void Viewport::initializeGL() {
    const GLenum error { glewInit() };
    if (GLEW_OK != error) {
        throw std::runtime_error { reinterpret_cast<const char*>(glewGetErrorString(error)) };
    }

    glClearColor(0.3, 1.0, 0.3, 1.0f);
    std::cout << "initialized OpenGL: " << glGetString(GL_VERSION) << std::endl;
}

void Viewport::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << "resized windget to " << width << "x" << height << std::endl;
}

void Viewport::paintGL() {
	std::cout << "Viewport::paintGL()" << std::endl;
    draw(0);
}

void Viewport::draw(float delta) {
    // nothing to do yet
}

}  // namespace gui
}  // namespace bgl
