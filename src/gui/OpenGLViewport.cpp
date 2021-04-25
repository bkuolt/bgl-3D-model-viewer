/**
 * @file viewport.cpp
 * @brief A simple OpenGL Qt Viewport
 */
#include "OpenGL.hpp"
#include "OpenGLViewport.hpp"

#include <iostream>


namespace bgl {
namespace gui {

OpenGLViewport::OpenGLViewport(QWidget *parent)
    : QOpenGLWidget(parent)
{}

void OpenGLViewport::setScene(const std::shared_ptr<Scene> &scene) {
    _scene = scene;
    // TODO: trigger repaint
}

std::shared_ptr<Scene> OpenGLViewport::getScene() noexcept {
    return _scene;
}

void OpenGLViewport::initializeGL() {
    if (GLEW_OK != glewInit()) {
        throw std::runtime_error { "could not initialize GLEW" };
    }

    glClearColor(0.3, 1.0, 0.3, 1.0f);
    std::cout << "initialized OpenGL: " << glGetString(GL_VERSION) << std::endl;
}

void OpenGLViewport::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << "resized windget to " << width << "x" << height << std::endl;
}

void OpenGLViewport::paintGL() {
    if (_scene) {
        _scene->render();
        this->update();  // triggers repaint()
    }
    // std::cout << "paintedGL()" << std::endl;
}

void OpenGLViewport::paintEvent(QPaintEvent *event) {
    // TODO: limit frame rate
    paintGL();
}

}  // namespace gui
}  // namespace bgl
