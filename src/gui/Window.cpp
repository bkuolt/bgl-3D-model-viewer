#include "OpenGLViewport.hpp"  // must be incuded first because og GLEW

#include <QGuiApplication>
#include <QScreen>

#include <QCoreApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QMainWindow>
#include <QKeyEvent>
#include <QtConcurrent>
#include <QImage>

#include <cassert>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

#include "Window.hpp"
#include "MenuBar.hpp"
#include "StatusBar.hpp"

////////////////////////////////


#undef USE_CONCURRENCY  // TODO: Cannot make QOpenGLContext current in a different thread

/////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

namespace bgl {
namespace gui {

using namespace std::string_literals;
using namespace std::chrono_literals;

namespace {

std::shared_ptr<QOpenGLTexture> LoadTexture(const std::filesystem::path &path) {
	std::cout << "Loading Texture..." << std::endl;
    QImage image { path.string().c_str() };
    if (image.isNull()) {
        throw std::runtime_error { "Could not load QImage "s + path.string() };
    }

    auto texture { std::make_shared<QOpenGLTexture>(image) };
    if (!texture->create()) {
        throw std::runtime_error { "Could not create QOpenGLTexture " };
    }
    return texture;
}

/**
 * @brief Returns the desktop size of the primary screen.
 */
QSize GetScreenSize() {
    QScreen * const screen { QGuiApplication::primaryScreen() };
    if (screen == nullptr) {
        throw std::runtime_error { "no screen found " };
    }
    const QRect &screenGeometry { screen->geometry() };
    return { screenGeometry.width(), screenGeometry.height() };
}

inline std::optional<std::filesystem::path> ChooseFile() {
    const QString fileName {
        QFileDialog::getOpenFileName(nullptr, "Load 3D Model", "/home/", "All Files (*)",
                                     nullptr, QFileDialog::Option::DontUseNativeDialog)
    };
    return fileName.isEmpty() ? std::nullopt
                              : std::optional<std::filesystem::path> { fileName.toStdString() };
}


auto ModelLoadTask = [] (Window *window, std::filesystem::path path) {
    MenuBar * const menuBar { dynamic_cast<MenuBar*>(window->menuBar()) };

    assert(menuBar != nullptr);
    menuBar->updateStatusBar(0);

    try {
		window->viewport()->makeCurrent();  // QOpenGLTexture::setData() requires a valid current context
        // TODO: load Model

#ifdef USE_CONCURRENCY
        std::this_thread::sleep_for(4s);  // TODO
        menuBar->updateStatusBar(75);
#endif  // USE_CONCURRENCY

#ifdef USE_CONCURRENCY
        std::this_thread::sleep_for(1s);  // TODO
#endif  // USE_CONCURRENCY
        menuBar->updateStatusBar(100);
    } catch (const std::exception &error) {
        QMessageBox::critical(nullptr, "Error", error.what());
    }
};

/* ----------------------------------------------------------- */

class SimpleViewport : public OpenGLViewport {
 public:
    explicit SimpleViewport(QWidget *parent = nullptr) 
        : OpenGLViewport { parent }
    {}

	virtual ~SimpleViewport() noexcept = default;

 protected:
    void paintGL() override {
        if(getScene())
            getScene()->draw();
        // TODO
    }
};

}  // anonymous namespace


Window::Window()
    : Window(GetScreenSize() * 0.75, "BGL QOpenGLWidget Demo")
{}

Window::Window(const QSize &size, const QString &title) {
    OpenGLViewport * const viewport { new SimpleViewport { this } };

    this->setWindowTitle(title);
    this->setFixedSize(size);

    this->setViewport(viewport  /* takes ownership */);

    QStatusBar * const statusBar { new StatusBar { this } };
    this->setStatusBar(statusBar /* takes ownership */);


    MenuBar * const menuBar { new MenuBar { this } };
    this->setMenuBar(menuBar /* takes ownership */);
    this->show();
}

void Window::setViewport(QOpenGLWidget *viewport) {
    assert(viewport);
    this->setCentralWidget(viewport);
}

void Window::setScene(const std::shared_ptr<bgl::Scene> &scene) noexcept {
    OpenGLViewport * const viewport { dynamic_cast<OpenGLViewport*>(this->viewport()) };  // TODO
	viewport->setScene(scene);
}

std::shared_ptr<bgl::Scene> Window::getScene() noexcept {
    OpenGLViewport * const viewport { dynamic_cast<OpenGLViewport*>(this->viewport()) };  // TODO
	return viewport->getScene();
}

void Window::loadModel(const std::filesystem::path &path) noexcept {
#ifdef USE_CONCURRENCY
    if (_imageLoadingTask.isStarted()) {
        _imageLoadingTask.cancel();
    }
    _imageLoadingTask = QtConcurrent::run(ModelLoadTask, this, path);
	if (!_imageLoadingTask.isStarted()) {
		QMessageBox::information(nullptr, "Error", "Could not start image loading thread.");
	}
#else
	ModelLoadTask(this, path);
#endif  // USE_CONCURRENCY
}

void Window::loadModel() noexcept {
    const std::optional<std::filesystem::path> path { ChooseFile() };
    if (path.has_value()) {
        this->loadModel(path.value());
    } else {
        QMessageBox::information(nullptr, "Warning", "No file chosen.");
    }
}

QOpenGLWidget* Window::viewport() noexcept {
    return dynamic_cast<QOpenGLWidget*>(centralWidget());
}

void Window::closeEvent(QCloseEvent *event) {
    std::cout << "closed window" << std::endl;
}

void Window::keyPressEvent(QKeyEvent *event) {

try {  // TODO
    const float rotation  { 5 };
    if ( !getScene()) return;

    ArcBall &camera = getScene()->camera();

	std::cout << "key pressed" << std::endl;

    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_Left:
            camera.rotate(-rotation, 0);
            break;
        case Qt::Key_Right:
            camera.rotate(rotation, 0);
            break;
        case Qt::Key_Up:
            camera.rotate(0, -rotation);
            break;
        case Qt::Key_Down:
            camera.rotate(0, rotation);
    }

    centralWidget()->repaint();

    } catch(std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

void Window::mouseMoveEvent(QMouseEvent *event) {
    std::cout << "mouseMoveEvent" << std::endl;
    // TODO
}

void Window::wheelEvent(QWheelEvent *event) {
    ArcBall &camera = getScene()->camera();

    std::cout << "wheel event" << std::endl;
    const float delta { (-event->angleDelta().y() / 120.0f) / 10.0f };  // TODO
    const float zoom { std::max(camera.getZoom() + delta, 1.0f) };
    camera.setZoom(zoom);

    centralWidget()->repaint();

}

}  // namespace gui
}  // namespace bgl
