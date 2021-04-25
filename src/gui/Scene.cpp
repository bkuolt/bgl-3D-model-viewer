#include "Scene.hpp"
#include <QVector2D>

namespace bgl {

namespace {

void DrawImage(QOpenGLTexture *texture,
               const QVector2D &position, const QVector2D &size) {
    // TODO
}

}  // anyonmous namespace

void Scene::setBackground(const std::shared_ptr<QOpenGLTexture> &background) {
    _background = background;  // TODO: get OpenGL texture from QImage
}

std::shared_ptr<QOpenGLTexture> Scene::getBackground() noexcept {
	return _background;  // TODO
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (_background) {
        DrawImage(_background.get(), { 0.0f, 0.0f }, { 1.0f, 1.0f });
    } else {
        // TODO: use clear color
    }
}

}  // namespace bgl
