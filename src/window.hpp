// Copyright 2021 Bastian Kuolt
#include "gfx/gfx.hpp"
#include "gfx/box.hpp"
#include "gfx/grid.hpp"
#include "gfx/camera.hpp"

//#undef Q_CC_GNU  //  removes "#warning To use GLEW with Qt, do not include <qopengl.h> or <QOpenGLFunctions> after glew.h"
#include <QApplication>
#include <QKeyEvent>


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


class GLViewport final : public Viewport {
 public:
	explicit GLViewport(QWidget *parent)
		: Viewport(parent)
	{}  
	// TODO(bkuolt): not movable, not copyable, destructor

	void on_render(float delta) override {
		static bool initialized { false };
		if (!initialized) {
			set_up_scene("./assets/models/housemedieval.obj");  // TODO(bkuolt)
			initialized = true;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		const mat4 PV { Scene.camera.getMatrix() };
		Scene.grid->render(PV);
		Scene.mesh->render(PV);
		Scene.box->render(PV);
	}
};


class SimpleWindow final : public bgl::Window {
 public:
	explicit SimpleWindow(const std::string &title)
		: bgl::Window(title) {
        static GLViewport viewport { this };
		this->setViewport(&viewport);
        this->show();
    }

	// TODO(bkuolt): not movable, not copyable, destructor

	bool event(QEvent *event) override {
		if (event->type()  == QEvent::KeyPress) {
			return keyEvent(reinterpret_cast<QKeyEvent*>(event));
		}

		return QMainWindow::event(event);
	}

 private:
	bool keyEvent(QKeyEvent *event) {
		switch (event->key()) {
			case Qt::Key_Escape:
				close();
				break;
			case Qt::Key_Left:
				Scene.camera.rotate(vec2(0, -0.5));
				break;
			case Qt::Key_Right:
				Scene.camera.rotate(vec2(0, 0.5));
				break;
		default:
			return QMainWindow::event(event);
		}

		_viewport->makeCurrent();
		_viewport->update();
		return true;
	}
};

}  // namespace bgl
