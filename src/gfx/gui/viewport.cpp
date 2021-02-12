/**
 * @file viewport.cpp
 * @brief A simple OpenGL Qt Viewport
 */
#include "GLViewport.hpp"

#include <GL/gl.h>
#include <QOpenGLWidget>

#include <iostream>


namespace bgl {

GLViewport::GLViewport(QWidget *parent)
    : QOpenGLWidget(parent)
{}

void GLViewport::initializeGL() {
    glClearColor(0.3, 1.0, 0.3, 1.0f);
    std::cout << "initialized OpenGL: " << glGetString(GL_VERSION) << std::endl;
}

void GLViewport::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << "resized windget to " << width << "x" << height << std::endl;
}

void GLViewport::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO(bkuolt): place draw code from bgl-demo here
    std::cout << "paintedGL()" << std::endl;
}

}  // namespace bgl
