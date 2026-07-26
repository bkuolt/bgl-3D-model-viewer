#include "window.hpp"

#include <QApplication>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QOpenGLFramebufferObjectFormat>
#include <QWheelEvent>
#include <algorithm>
#include <memory>

#include "gfx/gfx.hpp"
#include "gfx/gl.hpp"
#include "gfx/importer.hpp"

namespace bgl {

/* ------------------------------------ GLViewport ------------------------------------ */

GLViewport::GLViewport(QWidget* parent) : Viewport(parent) {}

GLViewport::~GLViewport() {
    cleanup();
}

void GLViewport::cleanup() {
    makeCurrent();
    _box.reset();
    _grid.reset();
    _model.reset();
    doneCurrent();
}

void GLViewport::initializeGL() {
    Viewport::initializeGL();
    if (QCoreApplication::arguments().size() > 1) {
        const std::filesystem::path path{QCoreApplication::arguments().at(1).toStdString()};

        _model = LoadModel(path);
        _camera.setFocus({0.0f, 0.0f, 0.0f});
        _camera.setPosition({0.0f, 1.0f, 2.0f});

        if (_model) {
            _box = std::make_shared<Box>(_model->getBoundingBox());
            _grid = std::make_shared<Grid>(0.125f, 40);
            const vec3 v{0.0f, -_model->getBoundingBox().getSize().y / 2.0f, 0.0f};
            _grid->translate(v);
        }

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
}

void GLViewport::on_render(float /*delta*/) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setMipmap(false);
    format.setSamples(0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!_model) {
        return;
    }

    const mat4 PV{_camera.matrix()};
    if (_grid) {
        _grid->render(PV);
    }
    if (_box) {
        _box->render(PV);
    }

    static DirectionalLight light{.direction = vec3{-1.0f, -1.0f, -1.0f},
                                  .diffuse = vec3{0.0f, 1.0f, 1.0f},
                                  .ambient = vec3{0.2f, 0.2f, 0.2f}};

    _model->render(PV, light);
}

/* ------------------------------------ SimpleWindow ------------------------------------ */

SimpleWindow::SimpleWindow(const std::string& title) : Window(title), _viewport(nullptr) {
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
            _viewport.camera().rotate(-rotation, 0.0f);
            break;
        case Qt::Key_Right:
            _viewport.camera().rotate(rotation, 0.0f);
            break;
        case Qt::Key_Up:
            _viewport.camera().rotate(0.0f, -rotation);
            break;
        case Qt::Key_Down:
            _viewport.camera().rotate(0.0f, rotation);
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
    const float zoom{std::max(_viewport.camera().getZoom() + delta, 1.0f)};
    _viewport.camera().setZoom(zoom);

    _viewport.makeCurrent();
    _viewport.update();
}

}  // namespace bgl
