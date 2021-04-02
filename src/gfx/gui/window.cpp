#include <iostream>
#include <future>    // std::call_once()
#include <memory>
#include <string>

#include "window.hpp"
#include "menu.hpp"

#include <QEvent>  // QEvent::KeyPress


namespace bgl {

Window::Window(const std::string &title) {
    this->setWindowTitle("BGL Demo");
    this->setFixedSize(1280, 720);
    this->show();

    auto menuBar = new MenuBar(*this);
    this->setMenuBar(menuBar);
}

void Window::setViewport(Viewport *viewport) {
    _viewport = viewport;
    _viewport->resize(1200, 800);
    _viewport->show();
    this->setCentralWidget(_viewport);
    // TODO: add panel
}

uvec2 Window::getSize() const noexcept {
    return { size().width(), size().height() };
}

void Window::render() {
	// TODO(bkuolt)
}

}  // namespace bgl
