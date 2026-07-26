#include "gui/window.hpp"

#include <QEvent>
#include <QGroupBox>
#include <QGuiApplication>
#include <QScreen>
#include <QStatusBar>
#include <memory>
#include <stdexcept>
#include <string>

#include "gfx/gl.hpp"
#include "gui/menu.hpp"
#include "gui/panel.hpp"

namespace bgl {

namespace {

QSize get_desktop_size() {
    const QScreen* const screen{QGuiApplication::primaryScreen()};
    if (screen == nullptr) {
        return {1024, 768};
    }
    const QRect& screenGeometry{screen->geometry()};
    return {screenGeometry.width(), screenGeometry.height()};
}

QStatusBar* get_dummy_status_bar() {
    auto* statusBar = new QStatusBar;
    statusBar->showMessage("Ready");
    return statusBar;
}

QMenuBar* get_dummy_menu_bar(QMainWindow& window) {
    return new MenuBar(window);
}

}  // anonymous namespace

Window::Window(const std::string& title) {
    this->setWindowTitle(QString::fromStdString(title));
    this->setMenuBar(get_dummy_menu_bar(*this));
    this->setStatusBar(get_dummy_status_bar());

    const QSize size{get_desktop_size()};
    this->setFixedSize(static_cast<int>(size.width() * 0.75),
                       static_cast<int>(size.height() * 0.75));
}

void Window::setViewport(Viewport* viewport) {
    _viewport = viewport;
    this->setCentralWidget(_viewport);
}

uvec2 Window::getSize() const noexcept {
    return {static_cast<GLuint>(size().width()), static_cast<GLuint>(size().height())};
}

void Window::render() {
    // Window render update loop
}

}  // namespace bgl
