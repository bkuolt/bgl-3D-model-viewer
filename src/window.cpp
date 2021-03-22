// Copyright 2021 Bastian Kuolt
#include "window.hpp"

#include "gfx/model.hpp"
#include "gfx/box.hpp"
#include "gfx/grid.hpp"
#include "gfx/camera.hpp"
#include "gfx/gui/menu.hpp"

#include <QApplication>
#include <QKeyEvent>
#include <QOpenGLFramebufferObject>

#include <memory>
#include <mutex>  // call_once


namespace bgl {

namespace {

/**
 * @brief The scene which is rendered to the viewport.
 */
struct {
	std::shared_ptr<Model> model;
	std::shared_ptr<Box> box;      // the bounding box of the 3D model
    std::shared_ptr<Grid> grid;

	DirectionalLight light;
    Camera camera;
} Scene;


void setUpScene(const std::shared_ptr<Model> &model) {
	Scene.model = model;
	Scene.camera.setViewCenter({ 0.0, 0.0, 0.0 });
	Scene.camera.setPosition({ 0.0, 1.0, 2.0 });

	Scene.box = std::make_shared<Box>(Scene.model->getBoundingBox());
	Scene.grid = std::make_shared<Grid>(0.125, 40);

    // repositions the grid under the bounding box
	const vec3 translation { 0.0, -Scene.model->getBoundingBox().getSize().y / 2.0, 0.0 };
	Scene.grid->translate(translation);

    Scene.light = DirectionalLight {
        .direction = vec3 { -1.0, -1.0, -1.0 },
        .diffuse = vec3 { 0.0, 1.0, 1.0 },
        .ambient = vec3 { 0.2f, 0.2f, 0.2f }
    };
}

void setUpDefaultScene() {
    const std::filesystem::path path { QCoreApplication::arguments().at(1).toStdString() };
    setUpScene(LoadModel(path));
}

void initGL() {
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

class SimpleMenuBar : public bgl::MenuBar {
 public:
    explicit SimpleMenuBar(Window &window)
        : bgl::MenuBar(window)
    {}

 protected:
     void onLoadModel(const std::shared_ptr<Model> &model) override {
        setUpScene(model);
    }
};

}  // anonymous namespace


GLViewport::GLViewport(QWidget *parent)
    : Viewport(parent)
{}

void GLViewport::onDraw(float delta) {
    static std::once_flag initialized;
    std::call_once(initialized, &setUpDefaultScene);

    initGL();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const mat4 PV { Scene.camera.getMatrix() };
    Scene.grid->render(PV);
    Scene.box->render(PV);
    Scene.model->render(PV, Scene.light);
}

SimpleWindow::SimpleWindow(const std::string &title)
    : bgl::Window(title), _viewport(this) {
    this->setViewport(&_viewport);
    this->show();

    auto menuBar = new SimpleMenuBar(*this);
    this->setMenuBar(menuBar);
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
        case Qt::Key_Left:
            Scene.camera.rotate(vec2(0, -0.5));
            break;
        case Qt::Key_Right:
            Scene.camera.rotate(vec2(0, 0.5));
            break;
    default:
        return QMainWindow::event(event);
    }

    _viewport.makeCurrent();
    _viewport.update();
    return true;
}

}  // namespace bgl
