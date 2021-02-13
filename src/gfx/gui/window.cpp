// Copyright 2021 Bastian Kuolt
#include "window.hpp"

#include <QEvent>  // QEvent::KeyPress

#include <iostream>
#include <future>    // std::call_once()
#include <memory>
#include <string>
#include <utility>   // std::swap()


namespace bgl {

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

void Window::setViewport(Viewport *viewport) {
    _viewport = viewport;
    _viewport->resize(1200, 800);
    _viewport->show();
    this->setCentralWidget(_viewport);

}

uvec2 Window::getSize() const noexcept {
    return { size().width(), size().height() };
}

void Window::render() {
	// TODO(bkuolt)
}

}  // namespace bgl
