#include "gfx/gfx.hpp"

#include <QApplication>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QOpenGLFramebufferObject>  // QOpenGLFramebufferObjectFormat

#include <algorithm>  // std::max()
#include <iostream>
#include <memory>     // std::shared_ptr

#include "window.hpp"

#include "gfx/model.hpp"
#include "gfx/box.hpp"
#include "gfx/grid.hpp"
#include "gfx/camera.hpp"


namespace bgl {

namespace {

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

}  // anonymous namespace

/* ------------------------------------ GLViewport ------------------------------------ */

GLViewport::GLViewport(QWidget *parent)
    : Viewport(parent)
{}

void GLViewport::draw(float delta) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setMipmap(false);
    format.setSamples(0);

    static bool initialized { false };
    if (!initialized) {
        const std::filesystem::path path { QCoreApplication::arguments().at(1).toStdString() };
        set_up_scene(path);
        initialized = true;
    }

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
}

bool handleKeyEvent(QMainWindow *window, QKeyEvent *event) {
    const float rotation  { 5 };

    switch (event->key()) {
        case Qt::Key_Escape:
            window->close();
            return true;
        case Qt::Key_Left:
            Scene.camera.rotate(-rotation, 0);
            break;
        case Qt::Key_Right:
            Scene.camera.rotate(rotation, 0);
            break;
        case Qt::Key_Up:
            Scene.camera.rotate(0, -rotation);
            break;
        case Qt::Key_Down:
            Scene.camera.rotate(0, rotation);
            break;
    default:
        return false;
    }

    return true;
}

/* ------------------------------------ SimpleWindow ------------------------------------ */

SimpleWindow::SimpleWindow(const std::string &title)
    : gui::Window(title) {
    this->setViewport(new GLViewport(this) /* QMainWindow takes ownership */);
    this->show();
}

bool SimpleWindow::event(QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        std::cout << "key press event" << std::endl;
        return handleKeyEvent(this, reinterpret_cast<QKeyEvent*>(event));
    }

    std::cout << "unhandled event: " << event->type() << std::endl;
    return Window::event(event);
}

void SimpleWindow::wheelEvent(QWheelEvent *event) {
    std::cout << "wheel event" << std::endl;
    const float delta { (-event->angleDelta().y() / 120.0f) / 10.0f };  // TODO
    const float zoom { std::max(Scene.camera.getZoom() + delta, 1.0f) };
    Scene.camera.setZoom(zoom);
    _viewport->draw();
}

}  // namespace bgl
