// Copyright 2021 Bastian Kuolt
#include "gfx/gfx.hpp"
#include "gfx/model.hpp"
#include "gfx/box.hpp"
#include "gfx/grid.hpp"
#include "gfx/camera.hpp"
#include "gfx/scene.hpp"

//#undef Q_CC_GNU  //  removes "#warning To use GLEW with Qt, do not include <qopengl.h> or <QOpenGLFunctions> after glew.h"
#include <QApplication>
#include <QKeyEvent>

#include <string>
#include <mutex>  // std::call_once()


namespace bgl {

namespace {


void set_up_scene(Scene &scene) {
	const std::filesystem::path path { QCoreApplication::arguments().at(1).toStdString() };

	auto model { LoadModel(path) };
	auto grid { std::make_shared<Grid>(0.125, 40) };
	auto bounding_box { std::make_shared<Box>(model->getBoundingBox()) };

	DirectionalLight light;
	light.direction = vec3 { -1.0, -1.0, -1.0 };
	light.diffuse = vec3 { 0.0, 1.0, 1.0 };
	light.ambient = vec3 { 0.2f, 0.2f, 0.2f };

	scene.add(model);
	scene.add(grid);
	scene.add(bounding_box);
	scene.add(std::make_shared<DirectionalLight>(light));

	// places origin of the grid at the center of the bounding box
	const vec3 v { 0.0, -model->getBoundingBox().getSize().y / 2.0, 0.0 };
	grid->translate(v);

	// sets up camera
	Camera &camera { scene.getCamera() };
	camera.setViewCenter({ 0.0, 0.0, 0.0 });
	camera.setPosition({ 0.0, 1.0, 2.0 });

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
	GLViewport(QWidget *parent,
	           std::shared_ptr<Scene> scene = std::make_shared<Scene>())
		: Viewport(parent), _scene(scene)
	{}

    virtual ~GLViewport() = default;
	// TODO(bkuolt): not movable, not copyable, destructor

	void on_render(float delta) override {
    	std::call_once(_initialized, &set_up_scene, *_scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_scene->render();
	}

	std::shared_ptr<Scene> getScene() const noexcept {
		return _scene;
	}

 private:
	std::shared_ptr<Scene> _scene;
	std::once_flag _initialized;
};


class SimpleWindow final : public bgl::Window {
 public:
    GLViewport _viewport;

	explicit SimpleWindow(const std::string &title)
		: bgl::Window(title), _viewport(this) {
		this->setViewport(&_viewport);
        this->show();
    }

    virtual ~SimpleWindow() noexcept = default;
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
                return true;
				break;
			case Qt::Key_Left:
				_viewport.getScene()->getCamera().rotate(vec2(0, -0.5));
				break;
			case Qt::Key_Right:
				_viewport.getScene()->getCamera().rotate(vec2(0, 0.5));
				break;
		default:
			return QMainWindow::event(event);
		}

		_viewport.makeCurrent();
		_viewport.update();
		return true;
	}
};

}  // namespace bgl
