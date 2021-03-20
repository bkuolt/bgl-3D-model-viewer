// Copyright 2020 Bastian Kuolt
#ifndef GFX_GFX_HPP_
#define GFX_GFX_HPP_

#include "gl.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "gui/window.hpp"

#include <filesystem>   // std::filesystem::path
#include <memory>       // std::shared_ptr
#include <ostream>      // std::ostream
#include <iostream>


#include <QOpenGLShaderProgram>   // TODO(bkuolt)

namespace bgl {

#ifdef __linux
namespace console_color {
    constexpr char blue[] = "\x1B[34m";
    constexpr char red[] = "\x1B[31m";
    constexpr char white[] = "\x1B[37m";
    constexpr char magenta[] = "\x1B[35m";
    constexpr char yellow[] = "\x1B[33m";
    constexpr char green[] =  "\x1B[32m";
}  // namespace console_color

#endif  // __linux

// vao must be bound!
inline void set_va_attribute(GLuint location, GLsizei size, GLenum type, GLsizei stride, GLsizei offset) {
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, type, GL_FALSE, stride, reinterpret_cast<void*>(offset));

    int error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "glVertexAttribPointer() failed for location " << location << " due to "
                  << gluErrorString(error) << std::endl;
        throw std::runtime_error { "glVertexAttribPointer() failed" };
    }
}

inline std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::filesystem::path &vs, const std::filesystem::path &fs) {
    const auto program { std::make_shared<QOpenGLShaderProgram>() };
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vs.string().c_str());
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, fs.string().c_str());
    // TODO: error handling
    return program;
}

}  // namespace bgl

#endif  // GFX_GFX_HPP_
