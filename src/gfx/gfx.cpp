#include "gfx/gfx.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "gfx/gl.hpp"

namespace bgl {

namespace {

std::filesystem::path resolve_shader_path(const std::filesystem::path& path) {
    if (std::filesystem::exists(path)) {
        return path;
    }
    const std::filesystem::path srcPath = std::filesystem::path("src") / path;
    if (std::filesystem::exists(srcPath)) {
        return srcPath;
    }
    const std::filesystem::path srcAssetsPath =
        std::filesystem::path("src/assets/shaders") / path.filename();
    if (std::filesystem::exists(srcAssetsPath)) {
        return srcAssetsPath;
    }
    const std::filesystem::path buildAssetsPath =
        std::filesystem::path("assets/shaders") / path.filename();
    if (std::filesystem::exists(buildAssetsPath)) {
        return buildAssetsPath;
    }
    return path;
}

}  // anonymous namespace

void set_va_attribute(GLint location, GLsizei size, GLenum type, GLsizei stride, GLsizei offset) {
    if (location < 0) {
        return;
    }

    glEnableVertexAttribArray(static_cast<GLuint>(location));
    glVertexAttribPointer(static_cast<GLuint>(location), size, type, GL_FALSE, stride,
                          reinterpret_cast<const void*>(offset));

    const GLenum error{glGetError()};
    if (error != GL_NO_ERROR) {
        std::ostringstream oss;
        oss << "glVertexAttribPointer() failed for location " << location << " due to "
            << reinterpret_cast<const char*>(gluErrorString(error));
        throw std::runtime_error{oss.str()};
    }
}

std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::filesystem::path& vs,
                                                  const std::filesystem::path& fs) {
    const auto resolvedVs = resolve_shader_path(vs);
    const auto resolvedFs = resolve_shader_path(fs);

    auto program = std::make_shared<QOpenGLShaderProgram>();
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          QString::fromStdString(resolvedVs.string()))) {
        throw std::runtime_error{"could not load or compile vertex shader from " +
                                 resolvedVs.string() + ": " + program->log().toStdString()};
    }
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          QString::fromStdString(resolvedFs.string()))) {
        throw std::runtime_error{"could not load or compile fragment shader from " +
                                 resolvedFs.string() + ": " + program->log().toStdString()};
    }
    if (!program->link()) {
        throw std::runtime_error{"could not link shader program for " + resolvedVs.string() +
                                 " and " + resolvedFs.string() + ": " +
                                 program->log().toStdString()};
    }
    return program;
}

std::shared_ptr<QOpenGLShaderProgram>
LoadProgram(const std::initializer_list<std::filesystem::path>& paths) {
    std::filesystem::path vs;
    std::filesystem::path fs;

    for (const auto& path : paths) {
        const auto extension{path.extension()};
        if (extension == ".vs") {
            vs = path;
        } else if (extension == ".fs") {
            fs = path;
        } else {
            throw std::runtime_error{"unsupported shader file extension for path: " +
                                     path.string()};
        }
    }
    return LoadProgram(vs, fs);
}

}  // namespace bgl
