#ifndef GFX_GFX_HPP_
#define GFX_GFX_HPP_

#include <filesystem>   // std::filesystem::path
#include <memory>       // std::shared_ptr

#include "gl.hpp"
#include "mesh.hpp"
#include "model.hpp"
// #include "gui/window.hpp"

#include <QOpenGLShaderProgram>  // NOLINT (glew issue)


namespace bgl {

void set_va_attribute(GLuint location, GLsizei size, GLenum type, GLsizei stride, GLsizei offset);
std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::filesystem::path &vs, const std::filesystem::path &fs);
std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::initializer_list<std::filesystem::path> &shaders);

}  // namespace bgl

#endif  // GFX_GFX_HPP_
