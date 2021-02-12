/**
 * @file viewport.cpp
 * @brief A simple OpenGL Qt Viewport
 */
#include "../gl/gl.hpp"
#include "GLViewport.hpp"


#include <QOpenGLWidget>
#include "../gl/gl.hpp"  // TODO(bkuolt)
#include <iostream>


namespace bgl {

GLViewport::GLViewport(QWidget *parent)
    : QOpenGLWidget(parent)
{}

void GLViewport::initializeGL() {
    const GLenum error { glewInit() };
    if (GLEW_OK != error) {
        throw std::runtime_error { reinterpret_cast<const char*>(glewGetErrorString(error)) };
    }

    glClearColor(0.3, 1.0, 0.3, 1.0f);
    std::cout << "initialized OpenGL: " << glGetString(GL_VERSION) << std::endl;
}

void GLViewport::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << "resized windget to " << width << "x" << height << std::endl;
}

void GLViewport::paintGL() {
    makeCurrent();
    on_render(1.0);
    // std::cout << "paintedGL()" << std::endl;
}

}  // namespace bgl
