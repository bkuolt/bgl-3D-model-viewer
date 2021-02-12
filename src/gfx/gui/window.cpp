// Copyright 2021 Bastian Kuolt
#include "window.hpp"

#include <QEvent>  // QEvent::KeyPress

#include <iostream>
#include <future>    // std::call_once()
#include <memory>
#include <string>
#include <utility>   // std::swap()


namespace bgl {

namespace {

class frame_counter {
 public:
    bool count() noexcept {
#if defined (USE_SDL2)
        _delta = (SDL_GetTicks() - _timestamp_render) / 1000.0f;
        _timestamp_render = SDL_GetTicks();
        ++_num_frames;

        const bool is_new_second { SDL_GetTicks() - _timestamp_fps >= 1000 };
        if (is_new_second) {
            _timestamp_fps = SDL_GetTicks();
            _fps = _num_frames;
        }

        return is_new_second;
#else 
        return false;  // TODO(bkuolt)
#endif
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

Window::Window(const std::string &title) {
    this->setWindowTitle("BGL Demo");
    this->setFixedSize(1200, 800);
    this->show();
}

/*
Window::Window(Window &&rhs) {
    swap(rhs);
}
*/

Window::~Window() noexcept {
    // TODO(bkuolt)
}

Window& Window::operator=(Window &&rhs) {
    swap(rhs);
    return *this;
}

void Window::swap(Window &rhs) noexcept {
    // TODO(bkuolt):
}

void Window::setViewport(GLViewport *p) {
    _viewport = p;
        // creates GL context
    _viewport->resize(1200, 800);
    _viewport->show();
    this->setCentralWidget(_viewport);

}

uvec2 Window::getSize() const noexcept {
    return { size().width(), size().height() };
}

void Window::render() {
  // static frame_counter frame_counter;
  // const bool changed { frame_counter.count() };
  // on_render(frame_counter.delta());

#if defined(USE_SDL2)
    if (changed) {
        // TODO(bkuolt): add TTF font rendering support
          std::cout << "\r" << frame_counter.fps() << " FPS" << std::flush;
    }
#endif  // 0
}

}  // namespace bgl
