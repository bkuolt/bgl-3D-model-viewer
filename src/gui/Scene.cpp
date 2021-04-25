// TODO DOxygen
// TODO INCLUDES

#include "Scene.hpp"

namespace bgl {

void Scene::setBackground(const std::shared_ptr<QOpenGLTexture> &background) {
    // TODO: get OpenGL texture from QImage
}

std::shared_ptr<QOpenGLTexture> Scene::getBackground() noexcept {
	// TODO
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // if has background
        // TODO: render texture
}

}  //namespace bgl

