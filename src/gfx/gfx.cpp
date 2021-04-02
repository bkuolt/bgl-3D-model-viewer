#include <string>
#include <sstream>

#include "gfx.hpp"


namespace bgl {

// vao must be bound!
void set_va_attribute(GLuint location, GLsizei size, GLenum type, GLsizei stride, GLsizei offset) {
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, type, GL_FALSE, stride, reinterpret_cast<void*>(offset));

    const GLenum error { glGetError() };
    if (error != GL_NO_ERROR) {
        std::ostringstream oss;
        oss << "glVertexAttribPointer() failed for location "
            << location << " due to " << gluErrorString(error);
        throw std::runtime_error { oss.str() };
    }
}

std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::filesystem::path &vs, const std::filesystem::path &fs) {
    const auto program { std::make_shared<QOpenGLShaderProgram>() };
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, vs.string().c_str())) {
        throw std::runtime_error { "could not add vertex shader" };
    }
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, fs.string().c_str())) {
        throw std::runtime_error { "could not add  fragment shader" };
    }
    return program;
}

std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::initializer_list<std::filesystem::path> &paths) {
    std::filesystem::path vs;
    std::filesystem::path fs;

    for (auto &path : paths) {
        const auto extension { path.extension() };
        if (extension == ".vs") {
            vs = path;
        } else if (extension == ".fs") {
            fs = path;
        } else {
            throw std::runtime_error { "unsupported shader file" };
        }
    }
    return LoadProgram(vs, fs);
}

}  // namespace bgl
