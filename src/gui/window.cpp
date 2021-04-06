#include <QEvent>          // QEvent::KeyPress
#include <QGuiApplication>
#include <QScreen>
#include <QStatusBar>

#include <iostream>
#include <future>    // std::call_once()
#include <memory>
#include <string>

#include "window.hpp"
#include "menu.hpp"


namespace bgl {

namespace {

/**
 * @brief Returns the desktop size of the primary screen.
 */
QSize get_desktop_size() {
    QScreen * const screen { QGuiApplication::primaryScreen() };
    if (screen == nullptr) {
        throw std::runtime_error { "no screen found " };
    }
    const QRect &screenGeometry { screen->geometry() };
    return { screenGeometry.width(), screenGeometry.height() };
}

QStatusBar* get_dummy_status_bar() {
    static QStatusBar * statusBar { new QStatusBar };
    statusBar->showMessage("TODO");
    return statusBar;
}

QMenuBar* get_dummy_menu_bar(QMainWindow &window) {
    static QMenuBar * menuBar { new MenuBar(window) };
    return menuBar;
}

}  // anonymous namespace

Window::Window(const std::string &title) {
    this->setWindowTitle("BGL Demo");

    this->setMenuBar(get_dummy_menu_bar(*this));
    this->setStatusBar(get_dummy_status_bar());
    // TODO: add panel

    const QSize size { get_desktop_size() };
    this->setFixedSize(size.width() * 0.75, size.height() * 0.75);
    this->show();
}

void Window::setViewport(Viewport *viewport) {
    _viewport = viewport;
    _viewport->resize(get_desktop_size());
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
