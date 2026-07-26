#include "gfx/gl.hpp"
#include "window.hpp"

#include <QApplication>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QOpenGLFramebufferObjectFormat>
#include <QWheelEvent>
#include <algorithm>
#include <memory>

#include "gfx/box.hpp"
#include "gfx/camera.hpp"
#include "gfx/gfx.hpp"
#include "gfx/grid.hpp"
#include "gfx/importer.hpp"
#include "gfx/model.hpp"

namespace bgl {

namespace {

struct {
    std::shared_ptr<Model> model;
    std::shared_ptr<Grid> grid;
    ArcBall camera;
    std::shared_ptr<Box> box;
} Scene;

void set_up_scene(const std::filesystem::path& path) {
    Scene.model = LoadModel(path);
    Scene.camera.setFocus({0.0f, 0.0f, 0.0f});
    Scene.camera.setPosition({0.0f, 1.0f, 2.0f});

    Scene.box = std::make_shared<Box>(Scene.model->getBoundingBox());

    Scene.grid = std::make_shared<Grid>(0.125f, 40);
    const vec3 v{0.0f, -Scene.model->getBoundingBox().getSize().y / 2.0f, 0.0f};
    Scene.grid->translate(v);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

GLViewport::GLViewport(QWidget* parent) : Viewport(parent) {}

void GLViewport::initializeGL() {
    Viewport::initializeGL();
    if (QCoreApplication::arguments().size() > 1) {
        const std::filesystem::path path{QCoreApplication::arguments().at(1).toStdString()};
        set_up_scene(path);
    }
}

void GLViewport::on_render(float /*delta*/) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setMipmap(false);
    format.setSamples(0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!Scene.model) {
        return;
    }

    const mat4 PV{Scene.camera.matrix()};
    if (Scene.grid) {
        Scene.grid->render(PV);
    }
    if (Scene.box) {
        Scene.box->render(PV);
    }

    static DirectionalLight light{.direction = vec3{-1.0f, -1.0f, -1.0f},
                                  .diffuse = vec3{0.0f, 1.0f, 1.0f},
                                  .ambient = vec3{0.2f, 0.2f, 0.2f}};

    Scene.model->render(PV, light);
}

/* ------------------------------------ SimpleWindow ------------------------------------ */

SimpleWindow::SimpleWindow(const std::string& title) : Window(title), _viewport(this) {
    this->setViewport(&_viewport);
    this->show();
}

bool SimpleWindow::event(QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        return keyEvent(reinterpret_cast<QKeyEvent*>(event));
    }

    return QMainWindow::event(event);
}

bool SimpleWindow::keyEvent(QKeyEvent* event) {
    constexpr float rotation = 5.0f;

    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            return true;
        case Qt::Key_Left:
            Scene.camera.rotate(-rotation, 0.0f);
            break;
        case Qt::Key_Right:
            Scene.camera.rotate(rotation, 0.0f);
            break;
        case Qt::Key_Up:
            Scene.camera.rotate(0.0f, -rotation);
            break;
        case Qt::Key_Down:
            Scene.camera.rotate(0.0f, rotation);
            break;
        default:
            return QMainWindow::event(event);
    }

    _viewport.makeCurrent();
    _viewport.update();
    return true;
}

void SimpleWindow::wheelEvent(QWheelEvent* event) {
    const float delta{(-event->angleDelta().y() / 120.0f) / 10.0f};
    const float zoom{std::max(Scene.camera.getZoom() + delta, 1.0f)};
    Scene.camera.setZoom(zoom);

    _viewport.makeCurrent();
    _viewport.update();
}

}  // namespace bgl
