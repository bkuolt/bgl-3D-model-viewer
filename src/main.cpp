// Copyright 2021 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "gfx/gfx.hpp"
#include "gfx/camera.hpp"

#undef Q_CC_GNU  //  removes "#warning To use GLEW with Qt, do not include <qopengl.h> or <QOpenGLFunctions> after glew.h"
#include <QApplication>
#include <QOpenGLContext>
#include <QKeyEvent>



namespace {

using namespace bgl;

// forward declaration
void set_up_scene(const std::filesystem::path &path);

struct {
	SharedMesh mesh;
	SharedGrid grid;
	Camera camera;
} Scene;


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
	}
};


class SimpleWindow final : public bgl::Window {
 public:
	explicit SimpleWindow(const std::string &title)
		: bgl::Window(title) {}

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

void signal_handler(int signal) {
	std::cerr << console_color::red << "\rrequested program termination" << std::endl;
	std::exit(EXIT_FAILURE);
}

}  // anomous namespace


int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cout << console_color::red << "usage: "
				  << console_color::white << "./bgl "
				  << console_color::blue << "<path-to-model>" << std::endl;
		return EXIT_FAILURE;
	}

	std::signal(SIGINT, signal_handler);
	std::signal(SIGHUP, signal_handler);

	try {
		QApplication app(argc, argv);
		SimpleWindow window { "BGL Model Viewer" };
		GLViewport viewport { &window };

		window.setViewport(&viewport);
		window.show();
		return app.exec();
	} catch (const std::exception &exception) {
		// TODO(bkuolt): show message box
		std::cout << console_color::red << "error: " << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


namespace {

void set_up_scene(const std::filesystem::path &path) {
	Scene.mesh = LoadMesh(path);
	Scene.camera.setViewCenter({ 0.0, 0.0, 0.0 });
	Scene.camera.setPosition({ 0.0, 1.0, 2.0 });

	Scene.grid = CreateGrid(0.125, 40);
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
