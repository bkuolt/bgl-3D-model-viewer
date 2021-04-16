#include <QEvent>          // QEvent::KeyPress
#include <QGuiApplication>
#include <QScreen>
#include <QStatusBar>
#include <QGroupBox>

#include <future>    // std::call_once()
#include <memory>
#include <string>

#include "window.hpp"
#include "menu.hpp"
#include "panel.hpp"


namespace bgl {
namespace gui {

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

inline QStatusBar* create_status_bar() {
    static QStatusBar * statusBar { new QStatusBar };
    statusBar->showMessage("TODO");
    return statusBar;
}

inline QMenuBar* create_menu_bar(QMainWindow &window) {
    return new MenuBar { window };  // TODO
}

inline QGroupBox* create_panel() {
    return nullptr;  // TODO
}

}  // anonymous namespace

Window::Window(const std::string &title) {
    QMenuBar * const menuBar { create_menu_bar(*this) };
    QStatusBar * const statusBar { create_status_bar() };

    this->setMenuBar(menuBar /* takes ownership */);
    this->setStatusBar(statusBar /* takes ownership */);
    this->setWindowTitle("3D Model Viewer Prototype");

    const QSize size { get_desktop_size() };
    this->setFixedSize(size.width() * 0.75, size.height() * 0.75);
    this->show();
}

void Window::setViewport(Viewport *viewport) {
    assert(viewport != nullptr);
    this->setCentralWidget(viewport /* takes ownership */);
    viewport->resize(get_desktop_size());
    viewport->show();
}

uvec2 Window::getSize() const noexcept {
    return { size().width(), size().height() };
}

}  // namespace gui
}  // namespace bgl
