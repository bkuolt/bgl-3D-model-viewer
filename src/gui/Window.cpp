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
#include "../gfx/model.hpp"
#include "../gfx/model.hpp"
#include "../gfx/box.hpp"
#include "../gfx/grid.hpp"
#include "../gfx/camera.hpp"

/////////////////////////////
namespace bgl {
namespace gui {

using namespace std::string_literals;
using namespace std::chrono_literals;

namespace {

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

////////////////////////////////////////////////////////////////////////////////////////////////////


struct {
	std::shared_ptr<Model> model;
	std::shared_ptr<Grid> grid;
	ArcBall camera;
	std::shared_ptr<Box> box;
} Scene;

void set_up_scene(const std::filesystem::path &path) {
	Scene.model = LoadModel(path);
	Scene.camera.setFocus({ 0.0, 0.0, 0.0 });
	Scene.camera.setPosition({ 0.0, 1.0, 2.0 });

	Scene.box = std::make_shared<Box>(Scene.model->getBoundingBox());

	Scene.grid = std::make_shared<Grid>(0.125, 40);
	const vec3 v { 0.0, -Scene.model->getBoundingBox().getSize().y / 2.0, 0.0 };
	Scene.grid->translate(v);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// enables OpenGL Antialiasing
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<QOpenGLTexture> LoadTexture(const std::filesystem::path &path) {
#if 0
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
#endif


    std::shared_ptr<bgl::Model> model;
  //  model = bgl::LoadModel(path);
    set_up_scene(path);




    return {};
}

}  // anonymous namespace

/* ----------------------------------------------------------- */

class VP : public OpenGLViewport {
 public:
    explicit VP(QWidget *parent = nullptr) 
        : OpenGLViewport(parent)
    {}

	virtual ~VP() noexcept = default;

 protected:
    void paintGL() override {
        this->makeCurrent();
        static bool initialized { false };
        if (!initialized) {
            const std::filesystem::path path { QCoreApplication::arguments().at(1).toStdString() };
            set_up_scene(path);
            initialized = true;
        }

        Scene.camera.rotate(2, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        const mat4 PV { Scene.camera.matrix() };
        Scene.grid->render(PV);
        Scene.box->render(PV);

        static DirectionalLight light {
            .direction = vec3 { -1.0, -1.0, -1.0 },
            .diffuse = vec3 { 0.0, 1.0, 1.0 },
            .ambient = vec3 { 0.2f, 0.2f, 0.2f }
        };

        Scene.model->render(PV, light);
        this->doneCurrent();
    }
};


Window::Window()
    : Window(GetScreenSize() * 0.75, "BGL QOpenGLWidget Demo")
{}

Window::Window(const QSize &size, const QString &title) {
    OpenGLViewport * const viewport { new VP { this } };

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
	dynamic_cast<OpenGLViewport*>(viewport())->setScene(scene);  // TODO
}

std::shared_ptr<bgl::Scene> Window::getScene() noexcept {
	return {};  // TODO
}

auto imageLoadTask = [] (Window *window, std::filesystem::path path) {
    MenuBar * const menuBar { dynamic_cast<MenuBar*>(window->menuBar()) };

    assert(menuBar != nullptr);
    menuBar->updateStatusBar(0);

    try {
		window->viewport()->makeCurrent();  // QOpenGLTexture::setData() requires a valid current context
        const auto texture { LoadTexture(path) };
#ifdef USE_CONCURRENCY
        std::this_thread::sleep_for(4s);  // TODO
        menuBar->updateStatusBar(75);
#endif  // USE_CONCURRENCY

        auto scene { std::make_shared<bgl::Scene>() };
        scene->setBackground(texture);
        window->setScene(scene);

#ifdef USE_CONCURRENCY
        std::this_thread::sleep_for(1s);  // TODO
#endif  // USE_CONCURRENCY
        menuBar->updateStatusBar(100);
    } catch (const std::exception &error) {
        QMessageBox::critical(nullptr, "Error", error.what());
    }
};

void Window::loadImage(const std::filesystem::path &path) noexcept {
    std::cout << "loading image " << path << std::endl;
#ifdef USE_CONCURRENCY
    if (_imageLoadingTask.isStarted()) {
        _imageLoadingTask.cancel();
    }
    _imageLoadingTask = QtConcurrent::run(imageLoadTask, this, path);
	if (!_imageLoadingTask.isStarted()) {
		QMessageBox::information(nullptr, "Error", "Could not start image loading thread.");
	}
#else
	imageLoadTask(this, path);
#endif  // USE_CONCURRENCY
}

void Window::loadImage() noexcept {
#ifdef DEBUG
	const std::filesystem::path path { "./assets/models/housemedieval.obj" };
	const auto absolute_path { std::filesystem::absolute(path) };
    this->loadImage(absolute_path);
#else
    const std::optional<std::filesystem::path> path { ChooseFile() };
    if (path.has_value()) {
        this->loadImage(path.value());
    } else {
        QMessageBox::information(nullptr, "Warning", "No file chosen.");
    }
#endif  // DEBUG
}

#ifndef DEBUG
void Window::loadModel(const std::filesystem::path &path) noexcept {
	// TODO
}

void Window::loadModel() noexcept {
	// TODO
}
#endif  // DEBUG

QOpenGLWidget* Window::viewport() noexcept {
    return dynamic_cast<QOpenGLWidget*>(centralWidget());
}

void Window::keyPressEvent(QKeyEvent *event) {
	std::cout << "key pressed" << std::endl;
    switch (event->key()) {
        case Qt::Key_Escape:
			std::cout << "terminating" << std::endl;
            close();
            break;
        default:
            centralWidget()->repaint();
    }
}

void Window::closeEvent(QCloseEvent *event) {
    std::cout << "closed window" << std::endl;
}

void Window::mouseMoveEvent(QMouseEvent *event) {
    std::cout << "mouseMoveEvent" << std::endl;
}

}  // namespace gui
}  // namespace bgl
