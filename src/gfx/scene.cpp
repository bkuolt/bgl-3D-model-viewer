// Coypright 20201 Bastian Kuolt
#include "scene.hpp"


namespace bgl {

void Scene::add(const std::shared_ptr<Model> &model) {
    if (model) {
        _models.insert(model);
    }
}

void Scene::add(const std::shared_ptr<DirectionalLight> &light) {
    if (light) {
        _lights.insert(model);
    }
}

void Scene::remove(const std::shared_ptr<Model> &model) {
    _models.erase(model);
}

void Scene::remove(const std::shared_ptr<DirectionalLight> &light) {
    _lights.erase(light);
}

void Scene::setCamera(const Camera &camera) {
    _camera = camera;
}

Camera& Scene::getCamera() noexcept {
    return _camera;
}

const Camera& Scene::getCamera() const noexcept {
    return _camera;
}

void Scene::render() {	
    for (std::shared_ptr<Model> &model : models) {
        const mat4 PV { _camera.getMatrix() };
        for (std::shared_ptr<Light> &light : lights) {
	    	model->render(PV, light);
        }
    }
}

}  // namespace bgl
