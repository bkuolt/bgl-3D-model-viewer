#pragma once

/**
 * @file gfx.hpp
 * @brief Core OpenGL helper functions and shader program loading facilities.
 */

#include <QOpenGLShaderProgram>
#include <filesystem>
#include <initializer_list>
#include <memory>

#include "gfx/gl.hpp"
#include "gfx/mesh.hpp"
#include "gfx/model.hpp"

namespace bgl {

void set_va_attribute(GLint location, GLsizei size, GLenum type, GLsizei stride, GLsizei offset);

std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::filesystem::path& vs,
                                                  const std::filesystem::path& fs);

std::shared_ptr<QOpenGLShaderProgram>
LoadProgram(const std::initializer_list<std::filesystem::path>& shaders);

}  // namespace bgl
