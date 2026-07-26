#include "gui/viewport.hpp"

#include <chrono>
#include <iostream>

#include "gfx/gl.hpp"

namespace bgl {

namespace {

class FrameCounter final {
  public:
    FrameCounter()
        : _lastFrameTime{std::chrono::steady_clock::now()}, _lastFpsTime{_lastFrameTime} {}

    bool count() noexcept {
        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<double> frameDuration = now - _lastFrameTime;
        _delta = frameDuration.count();
        _lastFrameTime = now;
        ++_numFrames;

        const std::chrono::duration<double> fpsDuration = now - _lastFpsTime;
        if (fpsDuration.count() >= 1.0) {
            _fps = _numFrames;
            _numFrames = 0;
            _lastFpsTime = now;
            return true;
        }

        return false;
    }

    [[nodiscard]] std::size_t fps() const noexcept {
        return _fps;
    }

    [[nodiscard]] double delta() const noexcept {
        return _delta;
    }

  private:
    std::chrono::steady_clock::time_point _lastFrameTime;
    std::chrono::steady_clock::time_point _lastFpsTime;
    std::size_t _numFrames{0};
    std::size_t _fps{0};
    double _delta{0.0};
};

}  // anonymous namespace

Viewport::Viewport(QWidget* parent) : QOpenGLWidget(parent) {}

void Viewport::initializeGL() {
    glewExperimental = GL_TRUE;
    const GLenum error{glewInit()};
    if (GLEW_OK != error) {
        std::cerr << "[bgl] GLEW init note: " << glewGetErrorString(error) << std::endl;
    }
    // Flush initial glGetError triggered by glewExperimental in core context
    while (glGetError() != GL_NO_ERROR) {
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    std::cout << "[bgl] OpenGL Initialized: " << glGetString(GL_VERSION) << std::endl;
}

void Viewport::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
}

void Viewport::paintGL() {
    static FrameCounter counter;
    const bool fpsUpdated{counter.count()};
    if (fpsUpdated) {
        std::cout << "\r[bgl] Performance: " << counter.fps() << " FPS" << std::flush;
    }

    makeCurrent();
    on_render(static_cast<float>(counter.delta()));
}

void Viewport::on_render(float /*delta*/) {
    // Virtual render callback implemented by derived Viewport classes
}

}  // namespace bgl
