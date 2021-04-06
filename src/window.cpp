#include "gfx/gfx.hpp"

#include <QApplication>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QOpenGLFramebufferObject>  // QOpenGLFramebufferObjectFormat

#include <algorithm>  // std::max()
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
	Camera camera;
	std::shared_ptr<Box> box;
} Scene;

void set_up_scene(const std::filesystem::path &path) {
	Scene.model = LoadModel(path);
	Scene.camera.setViewCenter({ 0.0, 0.0, 0.0 });
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

void GLViewport::on_render(float delta) {
    static QSize size { 1280, 720 };

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
    const mat4 PV { Scene.camera.getMatrix() };
    Scene.grid->render(PV);
    Scene.box->render(PV);

    static DirectionalLight light {
        .direction = vec3 { -1.0, -1.0, -1.0 },
        .diffuse = vec3 { 0.0, 1.0, 1.0 },
        .ambient = vec3 { 0.2f, 0.2f, 0.2f }
    };

    Scene.model->render(PV, light);
}

/* ------------------------------------ SimpleWindow ------------------------------------ */

SimpleWindow::SimpleWindow(const std::string &title)
    : bgl::Window(title), _viewport(this) {
    this->setViewport(&_viewport);
    this->show();
}

bool SimpleWindow::event(QEvent *event) {
    if (event->type()  == QEvent::KeyPress) {
        return keyEvent(reinterpret_cast<QKeyEvent*>(event));
    }

    return QMainWindow::event(event);
}

bool SimpleWindow::keyEvent(QKeyEvent *event) {
    const auto rotation = 0.5;

    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            return true;
        case Qt::Key_Left:
            Scene.camera.rotate(-rotation, Camera::RotationAxis::Y);
            break;
        case Qt::Key_Right:
            Scene.camera.rotate(rotation, Camera::RotationAxis::Y);
            break;
        case Qt::Key_Up:
            Scene.camera.rotate(-rotation, Camera::RotationAxis::Z);
            break;
        case Qt::Key_Down:
            Scene.camera.rotate(rotation, Camera::RotationAxis::Z);
            break;
    default:
        return QMainWindow::event(event);
    }

    _viewport.makeCurrent();
    _viewport.update();
    return true;
}

void SimpleWindow::wheelEvent(QWheelEvent *event) {
    const float delta { (-event->angleDelta().y() / 120.0f) / 10.0f };  // TODO
    const float zoom { std::max(Scene.camera.getZoom() + delta, 1.0f) };
    Scene.camera.setZoom(zoom);

    _viewport.makeCurrent();
    _viewport.update();
}

}  // namespace bgl
