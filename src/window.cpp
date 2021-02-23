// Copyright 2021 Bastian Kuolt
#include "window.hpp"

#include "gfx/gfx.hpp"
#include "gfx/box.hpp"
#include "gfx/quad.hpp"

#include "gfx/grid.hpp"
#include "gfx/camera.hpp"

#include <QKeyEvent>
#include <QFileInfo>
#include <QOpenGLFramebufferObject>

#include <memory>
#include <iostream>


namespace bgl {

namespace {

struct {
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Grid> grid;
	Camera camera;
	std::shared_ptr<Box> box;
} Scene;

void set_up_scene(const std::filesystem::path &path) {
	Scene.mesh = std::make_shared<Mesh>(path);
	Scene.camera.setViewCenter({ 0.0, 0.0, 0.0 });
	Scene.camera.setPosition({ 0.0, 1.0, 2.0 });
	Scene.box = std::make_shared<Box>(Scene.mesh->getBoundingBox());

	Scene.grid = std::make_shared<Grid>(0.125, 40);
	const vec3 v { 0.0, -Scene.mesh->getBoundingBox().getSize().y / 2.0, 0.0 };
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



GLViewport::GLViewport(QWidget *parent)
    : Viewport(parent) {
}

void GLViewport::on_render(float delta) {
    static bool initialized { false };
    if (!initialized) {
        set_up_scene("./assets/models/housemedieval.obj");  // TODO(bkuolt)

        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setMipmap(false);
        format.setSamples(0);

        _fbo = new QOpenGLFramebufferObject { { this->width(), this->height() }, format };
        initialized = true;
    }

    /* ------------------- First Pass ------------------- */
    _fbo->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const mat4 PV { Scene.camera.getMatrix() };
    Scene.grid->render(PV);
    Scene.mesh->render(PV);
    Scene.box->render(PV);

    const GLuint textureID { _fbo->texture() };
    _fbo->release();
    QOpenGLFramebufferObject::bindDefault();
    /* --------------------- Second Pass ---------------- */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawQuad(textureID);
}

void GLViewport::takeScreenshot() {
    makeCurrent();
    const QImage image { _fbo->toImage() };
    const QString path { QFileInfo("./screenshot.png").absoluteFilePath() };
    image.save(path);
    std::cout << "captured screenshot (" << image.width() << "x" << image.height() << ")" << std::endl;
}


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
    switch (event->key()) {
        case Qt::Key_Escape:
            close();
            return true;
            break;
        case Qt::Key_Left:
            Scene.camera.rotate(vec2(0, -0.5));
            break;
        case Qt::Key_Right:
            Scene.camera.rotate(vec2(0, 0.5));
            break;
        case Qt::Key_P:
            _viewport.takeScreenshot();
            break;
    default:
        return QMainWindow::event(event);
    }

    _viewport.makeCurrent();
    _viewport.update();
    return true;
}

}  // namespace bgl
