#include <cassert>
#include <iostream>

#include <QVector2D>
#include <QCoreApplication>


#include "Scene.hpp"

#include <QOpenGLFramebufferObject>

#include "../gfx/model.hpp"  // TODO
#include "../gfx/box.hpp"
#include "../gfx/grid.hpp"
#include "../gfx/camera.hpp"


namespace bgl {

void Scene::add(const std::shared_ptr<Model> &model) {
    assert(model != nullptr);

    _model = model;
	_box = std::make_shared<Box>(_model->getBoundingBox());

	_camera.setFocus({ 0.0, 0.0, 0.0 });
	_camera.setPosition({ 0.0, 1.0, 2.0 });

	_grid = std::make_shared<Grid>(0.125, 40);
	const vec3 v { 0.0, -_model->getBoundingBox().getSize().y / 2.0, 0.0 };
	_grid->translate(v);

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

void Scene::load(const std::filesystem::path &path) {
    auto model { LoadModel(path) };
    add(model);
    // TODO
}

ArcBall& Scene::camera() noexcept {
    return _camera;
}

void Scene::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const mat4 PV { _camera.matrix() };

    static DirectionalLight light {
        .direction = vec3 { -1.0, -1.0, -1.0 },
        .diffuse = vec3 { 0.0, 1.0, 1.0 },
        .ambient = vec3 { 0.2f, 0.2f, 0.2f }
    };
    // TODO: _grid->render(PV);
    _box->render(PV);
    _model->render(PV, light);


    // =================================================================================
    // =================================================================================
    // =================================================================================
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    const QSize size { viewport[2], viewport[3] };

#if 0
    QGLFramebufferObjectFormat format;
    format.setSamples(4);
    format.setAttachment(QGLFramebufferObject::CombinedDepthStencil);
#endif
    QOpenGLFramebufferObject fbo { size };

    fbo.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::cout << "is valid: " << fbo.isValid() << std::endl;

    // TODO: _grid->render(PV);
    _model->render(PV, light);
    _box->render(PV);

    fbo.release();

    QImage image = fbo.toImage();
    image.save("test_scr.png");
    std::cout << "FBO released" << std::endl;
// =================================================================================
// =================================================================================
// =================================================================================
        
}

}  // namespace bgl
