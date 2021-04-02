#ifndef GFX_GFX_HPP_
#define GFX_GFX_HPP_

#include <filesystem>   // std::filesystem::path
#include <memory>       // std::shared_ptr

#include "gl.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "gui/window.hpp"

#include <QOpenGLShaderProgram>  // NOLINT (glew issue)


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

void set_va_attribute(GLuint location, GLsizei size, GLenum type, GLsizei stride, GLsizei offset);
std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::filesystem::path &vs, const std::filesystem::path &fs);
std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::initializer_list<std::filesystem::path> &shaders);

}  // namespace bgl

#endif  // GFX_GFX_HPP_
